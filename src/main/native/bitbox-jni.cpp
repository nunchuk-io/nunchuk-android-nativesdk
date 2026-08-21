#include <jni.h>
#include <syslog.h>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>

#include <nunchuk.h>
#include "nunchukprovider.h"
#include "serializer.h"
#include "deserializer.h"
#include "string-wrapper.h"
#include "utils/bitbox/bitbox.hpp"

using namespace nunchuk;
using nunchuk::bitbox::AttestationStatus;
using nunchuk::bitbox::BitBoxBackup;
using nunchuk::bitbox::BitBoxDeviceInfo;
using nunchuk::bitbox::BitBoxError;
using nunchuk::bitbox::BitBoxFirmwareInfo;
using nunchuk::bitbox::BitBoxManager;
using nunchuk::bitbox::BitBoxMnemonicLength;
using nunchuk::bitbox::BitBoxProduct;
using nunchuk::bitbox::BitBoxStep;
using nunchuk::bitbox::BitBoxTransport;
using nunchuk::bitbox::BitBoxValue;
using nunchuk::bitbox::GetExtendedPublicKeyOptions;
using nunchuk::bitbox::InitializeResult;
using nunchuk::bitbox::ListBackupsResult;
using nunchuk::bitbox::WalletAddressOptions;

// Unlike LedgerManager, BitBoxManager needs a live Nunchuk& (it stores pairing data in
// the current account/chain app-state database) and that reference must outlive it. The
// provider replaces `nu` on every account/chain switch, so the manager is rebuilt
// whenever the underlying Nunchuk instance changes — keeping a manager bound to a freed
// Nunchuk would leave pairing writes pointing at a dead database.
static std::mutex g_bitboxMutex;
static std::unique_ptr<BitBoxManager> g_bitboxManager;
static Nunchuk *g_bitboxNunchuk = nullptr;

static BitBoxManager &bitboxManager() {
    Nunchuk *nu = NunchukProvider::get()->nu.get();
    if (nu == nullptr) {
        throw std::runtime_error("Nunchuk is not initialized");
    }
    std::lock_guard<std::mutex> lock(g_bitboxMutex);
    if (!g_bitboxManager || g_bitboxNunchuk != nu) {
        g_bitboxManager = std::make_unique<BitBoxManager>(*nu);
        g_bitboxNunchuk = nu;
        syslog(LOG_DEBUG, "[JNI] Created BitBoxManager");
    }
    return *g_bitboxManager;
}

static std::vector<unsigned char> toBytes(JNIEnv *env, jbyteArray data) {
    if (data == nullptr) return {};
    const jsize length = env->GetArrayLength(data);
    std::vector<unsigned char> bytes(static_cast<size_t>(length));
    if (length > 0) {
        env->GetByteArrayRegion(data, 0, length, reinterpret_cast<jbyte *>(bytes.data()));
    }
    return bytes;
}

static void setStringField(JNIEnv *env, jclass clazz, jobject instance, const char *setter,
                           const std::string &value) {
    jstring str = env->NewStringUTF(value.c_str());
    env->CallVoidMethod(instance, env->GetMethodID(clazz, setter, "(Ljava/lang/String;)V"), str);
    env->DeleteLocalRef(str);
}

// Build a com.nunchuk.android.model.BitBoxDeviceInfo from a native BitBoxDeviceInfo.
static jobject convert2JBitBoxDeviceInfo(JNIEnv *env, const BitBoxDeviceInfo &info) {
    jclass clazz = env->FindClass("com/nunchuk/android/model/BitBoxDeviceInfo");
    jobject instance = env->NewObject(clazz, env->GetMethodID(clazz, "<init>", "()V"));

    env->CallVoidMethod(instance, env->GetMethodID(clazz, "setProduct", "(I)V"),
                        static_cast<jint>(info.product));
    setStringField(env, clazz, instance, "setFirmwareVersion", info.firmware_version);
    env->CallVoidMethod(instance, env->GetMethodID(clazz, "setFirmwareUpgradeRequired", "(Z)V"),
                        static_cast<jboolean>(info.firmware_upgrade_required));
    setStringField(env, clazz, instance, "setName", info.name);
    env->CallVoidMethod(instance, env->GetMethodID(clazz, "setUnlocked", "(Z)V"),
                        static_cast<jboolean>(info.unlocked));
    env->CallVoidMethod(instance, env->GetMethodID(clazz, "setInitialized", "(Z)V"),
                        static_cast<jboolean>(info.initialized));
    env->CallVoidMethod(instance,
                        env->GetMethodID(clazz, "setMnemonicPassphraseEnabled", "(Z)V"),
                        static_cast<jboolean>(info.mnemonic_passphrase_enabled));
    setStringField(env, clazz, instance, "setSecurechipModel", info.securechip_model);
    setStringField(env, clazz, instance, "setPasswordStretchingAlgo",
                   info.password_stretching_algo);
    env->CallVoidMethod(instance, env->GetMethodID(clazz, "setBluetoothEnabled", "(Z)V"),
                        static_cast<jboolean>(info.bluetooth_enabled));
    setStringField(env, clazz, instance, "setBluetoothFirmwareVersion",
                   info.bluetooth_firmware_version);
    setStringField(env, clazz, instance, "setBluetoothFirmwareHash",
                   info.bluetooth_firmware_hash);
    return instance;
}

// Build a com.nunchuk.android.model.BitBoxStep from a native BitBoxStep.
// `type` and `interaction` are passed as enum ordinals (see BitBoxStep.kt); the native
// std::optional fields are flattened to null / the NO_PROGRESS sentinel.
static jobject convert2JBitBoxStep(JNIEnv *env, const BitBoxStep &step) {
    jclass stepClass = env->FindClass("com/nunchuk/android/model/BitBoxStep");
    jobject instance = env->NewObject(stepClass, env->GetMethodID(stepClass, "<init>", "()V"));

    env->CallVoidMethod(instance, env->GetMethodID(stepClass, "setType", "(I)V"),
                        static_cast<jint>(step.type));
    env->CallVoidMethod(instance, env->GetMethodID(stepClass, "setInteraction", "(I)V"),
                        static_cast<jint>(step.interaction));
    env->CallVoidMethod(instance, env->GetMethodID(stepClass, "setRetryAfterMs", "(I)V"),
                        static_cast<jint>(step.retry_after_ms));

    // writes: List<ByteArray>
    jclass arrayListClass = env->FindClass("java/util/ArrayList");
    jmethodID addMethod = env->GetMethodID(arrayListClass, "add", "(Ljava/lang/Object;)Z");
    jobject writesList = env->NewObject(arrayListClass,
                                        env->GetMethodID(arrayListClass, "<init>", "()V"));
    for (const auto &frame : step.writes) {
        jbyteArray arr = env->NewByteArray(static_cast<jsize>(frame.size()));
        env->SetByteArrayRegion(arr, 0, static_cast<jsize>(frame.size()),
                                reinterpret_cast<const jbyte *>(frame.data()));
        env->CallBooleanMethod(writesList, addMethod, arr);
        env->DeleteLocalRef(arr);
    }
    env->CallVoidMethod(instance,
                        env->GetMethodID(stepClass, "setWrites", "(Ljava/util/List;)V"),
                        writesList);
    env->DeleteLocalRef(writesList);

    // pairingCode: String?
    if (step.pairing_code.has_value()) {
        setStringField(env, stepClass, instance, "setPairingCode", *step.pairing_code);
    }

    // progress: Double, absent as BitBoxStep.NO_PROGRESS (-1.0)
    env->CallVoidMethod(instance, env->GetMethodID(stepClass, "setProgress", "(D)V"),
                        static_cast<jdouble>(step.progress.value_or(-1.0)));

    // error: BitBoxError?
    if (step.error.has_value()) {
        jclass errClass = env->FindClass("com/nunchuk/android/model/BitBoxError");
        jobject errInstance = env->NewObject(errClass,
                                            env->GetMethodID(errClass, "<init>", "()V"));
        env->CallVoidMethod(errInstance, env->GetMethodID(errClass, "setCode", "(I)V"),
                            static_cast<jint>(step.error->code));
        setStringField(env, errClass, errInstance, "setMessage", step.error->message);
        env->CallVoidMethod(errInstance, env->GetMethodID(errClass, "setDeviceCode", "(I)V"),
                            static_cast<jint>(step.error->device_code));
        env->CallVoidMethod(instance,
                            env->GetMethodID(stepClass, "setError",
                                             "(Lcom/nunchuk/android/model/BitBoxError;)V"),
                            errInstance);
        env->DeleteLocalRef(errInstance);
    }
    return instance;
}

// The string-shaped BitBoxValue variants (xpub, fingerprint, address, signature, PSBT,
// verified backup id). Non-string variants have their own typed getters below.
static std::string bitboxResultToString(const BitBoxValue &value) {
    return std::visit([](const auto &result) -> std::string {
        using T = std::decay_t<decltype(result)>;
        if constexpr (std::is_same_v<T, bitbox::GetExtendedPublicKeyResult>) {
            return result.extended_public_key;
        } else if constexpr (std::is_same_v<T, bitbox::GetMasterFingerprintResult>) {
            return result.master_fingerprint;
        } else if constexpr (std::is_same_v<T, bitbox::WalletAddressResult>) {
            return result.address;
        } else if constexpr (std::is_same_v<T, bitbox::SignMessageResult>) {
            return result.signature;
        } else if constexpr (std::is_same_v<T, bitbox::SignPsbtResult>) {
            return result.psbt;
        } else if constexpr (std::is_same_v<T, bitbox::CheckBackupResult>) {
            return result.backup_id;
        } else {
            return std::string();  // std::monostate or a non-string variant
        }
    }, value);
}

// The boolean-shaped BitBoxValue variants: isWalletRegistered and checkSdCard.
static bool bitboxResultToBoolean(const BitBoxValue &value) {
    return std::visit([](const auto &result) -> bool {
        using T = std::decay_t<decltype(result)>;
        if constexpr (std::is_same_v<T, bitbox::RegistrationResult>) {
            return result.registered;
        } else if constexpr (std::is_same_v<T, bitbox::SdCardStatusResult>) {
            return result.inserted;
        } else {
            return false;
        }
    }, value);
}

// Macro-free boilerplate is unreadable at this call count: every entry point is
// "resolve session, run one command, convert the step", with the two standard catches.
#define BITBOX_STEP_BODY(BLOCK)                                     \
    try {                                                           \
        BLOCK                                                       \
    } catch (BaseException &e) {                                    \
        Deserializer::convert2JException(env, e);                   \
        return nullptr;                                             \
    } catch (std::exception &e) {                                   \
        Deserializer::convertStdException2JException(env, e);        \
        return nullptr;                                             \
    }

extern "C" {

// region session lifecycle

/**
 * Creates a fresh session for [session_id] over [transport] and starts initialization.
 * BitBoxManager::forSession(id, transport) replaces any existing session, so this is
 * also the reconnect path: a transport disconnect invalidates the Noise session and the
 * app must come back through here rather than through bitboxResume.
 */
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_bitboxInitialize(
        JNIEnv *env, jobject thiz, jstring session_id, jint transport) {
    BITBOX_STEP_BODY({
        auto &session = bitboxManager().forSession(StringWrapper(env, session_id),
                                                  static_cast<BitBoxTransport>(transport));
        return convert2JBitBoxStep(env, session.initialize());
    })
}

JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_bitboxOnData(
        JNIEnv *env, jobject thiz, jstring session_id, jbyteArray data) {
    BITBOX_STEP_BODY({
        const auto bytes = toBytes(env, data);
        auto &session = bitboxManager().forSession(StringWrapper(env, session_id));
        return convert2JBitBoxStep(env, session.onData(bytes));
    })
}

/** Retries an expired request on the same session — never call this after a disconnect. */
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_bitboxResume(
        JNIEnv *env, jobject thiz, jstring session_id) {
    BITBOX_STEP_BODY({
        auto &session = bitboxManager().forSession(StringWrapper(env, session_id));
        return convert2JBitBoxStep(env, session.resume());
    })
}

/** Answers an AWAITING_USER step: whether the user confirmed the pairing code matches. */
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_bitboxConfirmPairing(
        JNIEnv *env, jobject thiz, jstring session_id, jboolean accepted) {
    BITBOX_STEP_BODY({
        auto &session = bitboxManager().forSession(StringWrapper(env, session_id));
        return convert2JBitBoxStep(env, session.confirmPairing(static_cast<bool>(accepted)));
    })
}

// endregion

// region key operations

JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_bitboxGetMasterFingerprint(
        JNIEnv *env, jobject thiz, jstring session_id) {
    BITBOX_STEP_BODY({
        auto &session = bitboxManager().forSession(StringWrapper(env, session_id));
        return convert2JBitBoxStep(env, session.getMasterFingerprint());
    })
}

JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_bitboxGetExtendedPublicKey(
        JNIEnv *env, jobject thiz, jstring session_id, jstring derivation_path,
        jboolean check_on_device) {
    BITBOX_STEP_BODY({
        auto &session = bitboxManager().forSession(StringWrapper(env, session_id));
        GetExtendedPublicKeyOptions options;
        options.check_on_device = static_cast<bool>(check_on_device);
        return convert2JBitBoxStep(
                env, session.getExtendedPublicKey(StringWrapper(env, derivation_path), options));
    })
}

JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_bitboxIsWalletRegistered(
        JNIEnv *env, jobject thiz, jstring session_id, jobject wallet) {
    BITBOX_STEP_BODY({
        auto &session = bitboxManager().forSession(StringWrapper(env, session_id));
        return convert2JBitBoxStep(
                env, session.isWalletRegistered(Serializer::convert2CWallet(env, wallet)));
    })
}

JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_bitboxRegisterWallet(
        JNIEnv *env, jobject thiz, jstring session_id, jobject wallet) {
    BITBOX_STEP_BODY({
        auto &session = bitboxManager().forSession(StringWrapper(env, session_id));
        return convert2JBitBoxStep(
                env, session.registerWallet(Serializer::convert2CWallet(env, wallet)));
    })
}

/**
 * Unlike Ledger, BitBox needs no registration HMAC from the app — the device reports its
 * own registration state via bitboxIsWalletRegistered.
 */
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_bitboxSignPsbt(
        JNIEnv *env, jobject thiz, jstring session_id, jobject wallet, jstring psbt) {
    BITBOX_STEP_BODY({
        auto &session = bitboxManager().forSession(StringWrapper(env, session_id));
        return convert2JBitBoxStep(env, session.signPsbt(Serializer::convert2CWallet(env, wallet),
                                                         StringWrapper(env, psbt)));
    })
}

JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_bitboxSignMessage(
        JNIEnv *env, jobject thiz, jstring session_id, jstring derivation_path,
        jstring message) {
    BITBOX_STEP_BODY({
        auto &session = bitboxManager().forSession(StringWrapper(env, session_id));
        return convert2JBitBoxStep(env, session.signMessage(StringWrapper(env, derivation_path),
                                                            StringWrapper(env, message)));
    })
}

JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_bitboxGetWalletAddress(
        JNIEnv *env, jobject thiz, jstring session_id, jobject wallet, jint address_index,
        jboolean change, jboolean check_on_device) {
    BITBOX_STEP_BODY({
        auto &session = bitboxManager().forSession(StringWrapper(env, session_id));
        WalletAddressOptions options;
        options.check_on_device = static_cast<bool>(check_on_device);
        options.change = static_cast<bool>(change);
        return convert2JBitBoxStep(
                env, session.getWalletAddress(Serializer::convert2CWallet(env, wallet),
                                              static_cast<uint32_t>(address_index), options));
    })
}

// endregion

// region device setup

JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_bitboxSetDeviceName(
        JNIEnv *env, jobject thiz, jstring session_id, jstring name) {
    BITBOX_STEP_BODY({
        auto &session = bitboxManager().forSession(StringWrapper(env, session_id));
        return convert2JBitBoxStep(env, session.setDeviceName(StringWrapper(env, name)));
    })
}

/** [mnemonic_length] is the word count itself (12 or 24), not an ordinal. */
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_bitboxCreateNewSeed(
        JNIEnv *env, jobject thiz, jstring session_id, jint mnemonic_length) {
    BITBOX_STEP_BODY({
        auto &session = bitboxManager().forSession(StringWrapper(env, session_id));
        return convert2JBitBoxStep(
                env, session.createNewSeed(static_cast<BitBoxMnemonicLength>(mnemonic_length)));
    })
}

/** Recovery words are shown on the device only; requires firmware 9.13+ as initial backup. */
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_bitboxShowMnemonic(
        JNIEnv *env, jobject thiz, jstring session_id) {
    BITBOX_STEP_BODY({
        auto &session = bitboxManager().forSession(StringWrapper(env, session_id));
        return convert2JBitBoxStep(env, session.showMnemonic());
    })
}

JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_bitboxRestoreFromMnemonic(
        JNIEnv *env, jobject thiz, jstring session_id) {
    BITBOX_STEP_BODY({
        auto &session = bitboxManager().forSession(StringWrapper(env, session_id));
        return convert2JBitBoxStep(env, session.restoreFromMnemonic());
    })
}

JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_bitboxCheckSdCard(
        JNIEnv *env, jobject thiz, jstring session_id) {
    BITBOX_STEP_BODY({
        auto &session = bitboxManager().forSession(StringWrapper(env, session_id));
        return convert2JBitBoxStep(env, session.checkSdCard());
    })
}

JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_bitboxInsertSdCard(
        JNIEnv *env, jobject thiz, jstring session_id) {
    BITBOX_STEP_BODY({
        auto &session = bitboxManager().forSession(StringWrapper(env, session_id));
        return convert2JBitBoxStep(env, session.insertSdCard());
    })
}

JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_bitboxListBackups(
        JNIEnv *env, jobject thiz, jstring session_id) {
    BITBOX_STEP_BODY({
        auto &session = bitboxManager().forSession(StringWrapper(env, session_id));
        return convert2JBitBoxStep(env, session.listBackups());
    })
}

JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_bitboxRestoreBackup(
        JNIEnv *env, jobject thiz, jstring session_id, jstring backup_id) {
    BITBOX_STEP_BODY({
        auto &session = bitboxManager().forSession(StringWrapper(env, session_id));
        return convert2JBitBoxStep(env, session.restoreBackup(StringWrapper(env, backup_id)));
    })
}

// endregion

// region device management

JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_bitboxChangePassword(
        JNIEnv *env, jobject thiz, jstring session_id) {
    BITBOX_STEP_BODY({
        auto &session = bitboxManager().forSession(StringWrapper(env, session_id));
        return convert2JBitBoxStep(env, session.changePassword());
    })
}

JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_bitboxSetMnemonicPassphraseEnabled(
        JNIEnv *env, jobject thiz, jstring session_id, jboolean enabled) {
    BITBOX_STEP_BODY({
        auto &session = bitboxManager().forSession(StringWrapper(env, session_id));
        return convert2JBitBoxStep(
                env, session.setMnemonicPassphraseEnabled(static_cast<bool>(enabled)));
    })
}

JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_bitboxCreateBackup(
        JNIEnv *env, jobject thiz, jstring session_id) {
    BITBOX_STEP_BODY({
        auto &session = bitboxManager().forSession(StringWrapper(env, session_id));
        return convert2JBitBoxStep(env, session.createBackup());
    })
}

/** Pass silent = false to verify a backup interactively; the result is the backup id. */
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_bitboxCheckBackup(
        JNIEnv *env, jobject thiz, jstring session_id, jboolean silent) {
    BITBOX_STEP_BODY({
        auto &session = bitboxManager().forSession(StringWrapper(env, session_id));
        return convert2JBitBoxStep(env, session.checkBackup(static_cast<bool>(silent)));
    })
}

/** Terminal step is REBOOT: write the frames, then expect the device to disconnect. */
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_bitboxFactoryReset(
        JNIEnv *env, jobject thiz, jstring session_id) {
    BITBOX_STEP_BODY({
        auto &session = bitboxManager().forSession(StringWrapper(env, session_id));
        return convert2JBitBoxStep(env, session.factoryReset());
    })
}

// endregion

// region firmware upgrade

/**
 * Reads a signed firmware file without touching the device, so the app can check the
 * product matches the connected BitBox before starting an upgrade.
 */
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_bitboxInspectFirmware(
        JNIEnv *env, jobject thiz, jbyteArray signed_firmware) {
    try {
        const auto bytes = toBytes(env, signed_firmware);
        const BitBoxFirmwareInfo info = bitbox::InspectFirmware(bytes);

        jclass clazz = env->FindClass("com/nunchuk/android/model/BitBoxFirmwareInfo");
        jobject instance = env->NewObject(clazz, env->GetMethodID(clazz, "<init>", "()V"));
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setProduct", "(I)V"),
                            static_cast<jint>(info.product));
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setMonotonicVersion", "(J)V"),
                            static_cast<jlong>(info.monotonic_version));
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setFirmwareSize", "(J)V"),
                            static_cast<jlong>(info.firmware_size));
        return instance;
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return nullptr;
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return nullptr;
    }
}

/** Reboots the device into its bootloader; expect a disconnect, then reconnect there. */
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_bitboxEnterFirmwareUpgrade(
        JNIEnv *env, jobject thiz, jstring session_id, jbyteArray signed_firmware) {
    BITBOX_STEP_BODY({
        const auto bytes = toBytes(env, signed_firmware);
        auto &session = bitboxManager().forSession(StringWrapper(env, session_id));
        return convert2JBitBoxStep(env, session.enterFirmwareUpgrade(bytes));
    })
}

/**
 * Opens a bootloader session for [session_id] — this replaces any normal session under
 * the same id, so a fresh bitboxInitialize is required afterwards.
 */
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_bitboxUpgradeFirmware(
        JNIEnv *env, jobject thiz, jstring session_id, jint product,
        jbyteArray signed_firmware) {
    BITBOX_STEP_BODY({
        const auto bytes = toBytes(env, signed_firmware);
        auto &session = bitboxManager().forBootloaderSession(
                StringWrapper(env, session_id), static_cast<BitBoxProduct>(product));
        return convert2JBitBoxStep(env, session.upgradeFirmware(bytes));
    })
}

JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_bitboxBootloaderOnData(
        JNIEnv *env, jobject thiz, jstring session_id, jbyteArray data) {
    BITBOX_STEP_BODY({
        const auto bytes = toBytes(env, data);
        auto &session = bitboxManager().forBootloaderSession(StringWrapper(env, session_id));
        return convert2JBitBoxStep(env, session.onData(bytes));
    })
}

/** Boots the installed firmware — used when a firmware upload is rejected. */
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_bitboxBootloaderReboot(
        JNIEnv *env, jobject thiz, jstring session_id) {
    BITBOX_STEP_BODY({
        auto &session = bitboxManager().forBootloaderSession(StringWrapper(env, session_id));
        return convert2JBitBoxStep(env, session.reboot());
    })
}

// endregion

// region results

JNIEXPORT jstring JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_bitboxResultString(
        JNIEnv *env, jobject thiz, jstring session_id) {
    try {
        auto &session = bitboxManager().forSession(StringWrapper(env, session_id));
        return env->NewStringUTF(bitboxResultToString(session.result()).c_str());
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return nullptr;
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return nullptr;
    }
}

JNIEXPORT jboolean JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_bitboxResultBoolean(
        JNIEnv *env, jobject thiz, jstring session_id) {
    try {
        auto &session = bitboxManager().forSession(StringWrapper(env, session_id));
        return static_cast<jboolean>(bitboxResultToBoolean(session.result()));
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return JNI_FALSE;
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return JNI_FALSE;
    }
}

/** The result of bitboxInitialize: device info plus the attestation verdict. */
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_bitboxInitializeResult(
        JNIEnv *env, jobject thiz, jstring session_id) {
    BITBOX_STEP_BODY({
        auto &session = bitboxManager().forSession(StringWrapper(env, session_id));
        const auto value = session.result();
        if (!std::holds_alternative<InitializeResult>(value)) return nullptr;
        const auto &result = std::get<InitializeResult>(value);

        jclass clazz = env->FindClass("com/nunchuk/android/model/BitBoxInitializeResult");
        jobject instance = env->NewObject(clazz, env->GetMethodID(clazz, "<init>", "()V"));
        jobject device = convert2JBitBoxDeviceInfo(env, result.device);
        env->CallVoidMethod(instance,
                            env->GetMethodID(clazz, "setDevice",
                                             "(Lcom/nunchuk/android/model/BitBoxDeviceInfo;)V"),
                            device);
        env->DeleteLocalRef(device);
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setAttestation", "(I)V"),
                            static_cast<jint>(result.attestation));
        setStringField(env, clazz, instance, "setAttestationMessage", result.attestation_message);
        return instance;
    })
}

/** The result of bitboxListBackups: the backups found on the inserted microSD card. */
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_bitboxListBackupsResult(
        JNIEnv *env, jobject thiz, jstring session_id) {
    BITBOX_STEP_BODY({
        auto &session = bitboxManager().forSession(StringWrapper(env, session_id));
        jclass arrayListClass = env->FindClass("java/util/ArrayList");
        jmethodID addMethod = env->GetMethodID(arrayListClass, "add", "(Ljava/lang/Object;)Z");
        jobject list = env->NewObject(arrayListClass,
                                      env->GetMethodID(arrayListClass, "<init>", "()V"));

        const auto value = session.result();
        if (std::holds_alternative<ListBackupsResult>(value)) {
            jclass clazz = env->FindClass("com/nunchuk/android/model/BitBoxBackup");
            jmethodID ctor = env->GetMethodID(clazz, "<init>", "()V");
            for (const BitBoxBackup &backup : std::get<ListBackupsResult>(value).backups) {
                jobject instance = env->NewObject(clazz, ctor);
                setStringField(env, clazz, instance, "setId", backup.id);
                setStringField(env, clazz, instance, "setName", backup.name);
                env->CallVoidMethod(instance, env->GetMethodID(clazz, "setTimestamp", "(J)V"),
                                    static_cast<jlong>(backup.timestamp));
                env->CallBooleanMethod(list, addMethod, instance);
                env->DeleteLocalRef(instance);
            }
        }
        return list;
    })
}

/** The device info cached on the session — valid once initialization has completed. */
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_bitboxDeviceInfo(
        JNIEnv *env, jobject thiz, jstring session_id) {
    BITBOX_STEP_BODY({
        auto &session = bitboxManager().forSession(StringWrapper(env, session_id));
        return convert2JBitBoxDeviceInfo(env, session.deviceInfo());
    })
}

// endregion

// region signer helpers

/** The derivation path a BitBox signs health-check messages with. */
JNIEXPORT jstring JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_getBitBoxSignMessagePath(
        JNIEnv *env, jobject thiz, jobject signer) {
    try {
        const auto path = bitbox::GetBitBoxSignMessagePath(
                Serializer::convert2CSigner(env, signer));
        return env->NewStringUTF(path.c_str());
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return nullptr;
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return nullptr;
    }
}

/** The compact-signature P2PKH address matching getBitBoxSignMessagePath. */
JNIEXPORT jstring JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_getBitBoxSignMessageAddress(
        JNIEnv *env, jobject thiz, jobject signer) {
    try {
        const auto address = bitbox::GetBitBoxSignMessageAddress(
                Serializer::convert2CSigner(env, signer));
        return env->NewStringUTF(address.c_str());
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return nullptr;
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return nullptr;
    }
}

// endregion

}  // extern "C"
