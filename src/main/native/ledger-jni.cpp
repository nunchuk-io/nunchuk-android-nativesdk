#include <jni.h>
#include <syslog.h>
#include <string>
#include <variant>
#include <vector>

#include <nunchuk.h>
#include "nunchukprovider.h"
#include "serializer.h"
#include "deserializer.h"
#include "string-wrapper.h"
#include "utils/ledger/ledger_manager.hpp"

using namespace nunchuk;
using nunchuk::ledger::LedgerManager;
using nunchuk::ledger::LedgerStep;
using nunchuk::ledger::LedgerTransport;
using nunchuk::ledger::LedgerValue;
using nunchuk::ledger::RegisteredWallet;
using nunchuk::ledger::WalletAddressOptions;

// One manager for the whole process. Sessions are keyed by a stable session_id
// (BLE address / USB device name) supplied by the app; each physical Ledger
// connection reuses the same id. The manager itself is thread-safe.
static LedgerManager g_ledgerManager;

// Build a com.nunchuk.android.model.LedgerStep from a native LedgerStep.
// `type` and `interaction` are passed as enum ordinals (see LedgerStep.kt).
static jobject convert2JLedgerStep(JNIEnv *env, const LedgerStep &step) {
    jclass stepClass = env->FindClass("com/nunchuk/android/model/LedgerStep");
    jmethodID stepCtor = env->GetMethodID(stepClass, "<init>", "()V");
    jobject instance = env->NewObject(stepClass, stepCtor);

    env->CallVoidMethod(instance, env->GetMethodID(stepClass, "setType", "(I)V"),
                        static_cast<jint>(step.type));
    env->CallVoidMethod(instance, env->GetMethodID(stepClass, "setInteraction", "(I)V"),
                        static_cast<jint>(step.interaction));

    // writes: List<ByteArray>
    jclass arrayListClass = env->FindClass("java/util/ArrayList");
    jmethodID listCtor = env->GetMethodID(arrayListClass, "<init>", "()V");
    jmethodID addMethod = env->GetMethodID(arrayListClass, "add", "(Ljava/lang/Object;)Z");
    jobject writesList = env->NewObject(arrayListClass, listCtor);
    for (const auto &frame : step.writes) {
        jbyteArray arr = env->NewByteArray(static_cast<jsize>(frame.size()));
        env->SetByteArrayRegion(arr, 0, static_cast<jsize>(frame.size()),
                                reinterpret_cast<const jbyte *>(frame.data()));
        env->CallBooleanMethod(writesList, addMethod, arr);
        env->DeleteLocalRef(arr);
    }
    env->CallVoidMethod(instance, env->GetMethodID(stepClass, "setWrites", "(Ljava/util/List;)V"),
                        writesList);
    env->DeleteLocalRef(writesList);

    // error: LedgerError?
    if (step.error.has_value()) {
        jclass errClass = env->FindClass("com/nunchuk/android/model/LedgerError");
        jmethodID errCtor = env->GetMethodID(errClass, "<init>", "()V");
        jobject errInstance = env->NewObject(errClass, errCtor);
        env->CallVoidMethod(errInstance, env->GetMethodID(errClass, "setCode", "(I)V"),
                            static_cast<jint>(step.error->code));
        jstring msg = env->NewStringUTF(step.error->message.c_str());
        env->CallVoidMethod(errInstance, env->GetMethodID(errClass, "setMessage",
                                                          "(Ljava/lang/String;)V"), msg);
        env->CallVoidMethod(errInstance, env->GetMethodID(errClass, "setStatusWord", "(I)V"),
                            static_cast<jint>(step.error->status_word));
        env->CallVoidMethod(instance, env->GetMethodID(stepClass, "setError",
                                                       "(Lcom/nunchuk/android/model/LedgerError;)V"),
                            errInstance);
        env->DeleteLocalRef(msg);
        env->DeleteLocalRef(errInstance);
    }
    return instance;
}

// Every LedgerValue variant carries a single string payload; extract it.
static std::string ledgerResultToString(const LedgerValue &value) {
    return std::visit([](const auto &result) -> std::string {
        using T = std::decay_t<decltype(result)>;
        if constexpr (std::is_same_v<T, ledger::GetExtendedPublicKeyResult>) {
            return result.extended_public_key;
        } else if constexpr (std::is_same_v<T, ledger::GetMasterFingerprintResult>) {
            return result.master_fingerprint;
        } else if constexpr (std::is_same_v<T, ledger::SignMessageResult>) {
            return result.signature;
        } else if constexpr (std::is_same_v<T, ledger::SignPsbtResult>) {
            return result.psbt;
        } else if constexpr (std::is_same_v<T, ledger::RegisteredWalletResult>) {
            return result.hmac;
        } else if constexpr (std::is_same_v<T, ledger::WalletAddressResult>) {
            return result.address;
        } else {
            return std::string();  // std::monostate: no result yet
        }
    }, value);
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_ledgerGetMasterFingerprint(
        JNIEnv *env, jobject thiz, jstring session_id, jint transport) {
    try {
        auto &session = g_ledgerManager.forSession(StringWrapper(env, session_id),
                                                   static_cast<LedgerTransport>(transport));
        return convert2JLedgerStep(env, session.getMasterFingerprint());
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return nullptr;
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return nullptr;
    }
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_ledgerGetExtendedPublicKey(
        JNIEnv *env, jobject thiz, jstring session_id, jint transport,
        jint wallet_type, jint address_type, jint index) {
    try {
        auto &session = g_ledgerManager.forSession(StringWrapper(env, session_id),
                                                   static_cast<LedgerTransport>(transport));
        auto step = session.getExtendedPublicKey(
                Serializer::convert2CWalletType(wallet_type),
                Serializer::convert2CAddressType(address_type),
                index);
        return convert2JLedgerStep(env, step);
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return nullptr;
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return nullptr;
    }
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_ledgerSignMessage(
        JNIEnv *env, jobject thiz, jstring session_id, jint transport,
        jstring derivation_path, jstring message) {
    try {
        auto &session = g_ledgerManager.forSession(StringWrapper(env, session_id),
                                                   static_cast<LedgerTransport>(transport));
        auto step = session.signMessage(StringWrapper(env, derivation_path),
                                        StringWrapper(env, message));
        return convert2JLedgerStep(env, step);
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return nullptr;
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return nullptr;
    }
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_ledgerRegisterWallet(
        JNIEnv *env, jobject thiz, jstring session_id, jint transport, jobject wallet) {
    try {
        auto &session = g_ledgerManager.forSession(StringWrapper(env, session_id),
                                                   static_cast<LedgerTransport>(transport));
        auto step = session.registerWallet(Serializer::convert2CWallet(env, wallet));
        return convert2JLedgerStep(env, step);
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return nullptr;
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return nullptr;
    }
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_ledgerSignPsbt(
        JNIEnv *env, jobject thiz, jstring session_id, jint transport,
        jobject wallet, jstring hmac, jstring psbt) {
    try {
        auto &session = g_ledgerManager.forSession(StringWrapper(env, session_id),
                                                   static_cast<LedgerTransport>(transport));
        RegisteredWallet registered(Serializer::convert2CWallet(env, wallet),
                                    StringWrapper(env, hmac));
        auto step = session.signPsbt(registered, StringWrapper(env, psbt));
        return convert2JLedgerStep(env, step);
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return nullptr;
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return nullptr;
    }
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_ledgerGetWalletAddress(
        JNIEnv *env, jobject thiz, jstring session_id, jint transport,
        jobject wallet, jstring hmac, jint address_index, jboolean change) {
    try {
        auto &session = g_ledgerManager.forSession(StringWrapper(env, session_id),
                                                   static_cast<LedgerTransport>(transport));
        RegisteredWallet registered(Serializer::convert2CWallet(env, wallet),
                                    StringWrapper(env, hmac));
        WalletAddressOptions options;
        options.check_on_device = true;
        options.change = static_cast<bool>(change);
        auto step = session.getWalletAddress(registered,
                                             static_cast<uint32_t>(address_index), options);
        return convert2JLedgerStep(env, step);
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return nullptr;
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return nullptr;
    }
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_ledgerOnData(
        JNIEnv *env, jobject thiz, jstring session_id, jbyteArray data) {
    try {
        jsize length = env->GetArrayLength(data);
        std::vector<unsigned char> bytes(static_cast<size_t>(length));
        if (length > 0) {
            env->GetByteArrayRegion(data, 0, length, reinterpret_cast<jbyte *>(bytes.data()));
        }
        auto &session = g_ledgerManager.forSession(StringWrapper(env, session_id));
        return convert2JLedgerStep(env, session.onData(bytes));
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return nullptr;
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return nullptr;
    }
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_ledgerResume(
        JNIEnv *env, jobject thiz, jstring session_id) {
    try {
        auto &session = g_ledgerManager.forSession(StringWrapper(env, session_id));
        return convert2JLedgerStep(env, session.resume());
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return nullptr;
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return nullptr;
    }
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_ledgerResultString(
        JNIEnv *env, jobject thiz, jstring session_id) {
    try {
        auto &session = g_ledgerManager.forSession(StringWrapper(env, session_id));
        return env->NewStringUTF(ledgerResultToString(session.result()).c_str());
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return nullptr;
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return nullptr;
    }
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_getLedgerWalletHmac(
        JNIEnv *env, jobject thiz, jstring wallet_id) {
    try {
        auto hmac = NunchukProvider::get()->nu->GetLedgerWalletHmac(StringWrapper(env, wallet_id));
        return env->NewStringUTF(hmac.c_str());
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return nullptr;
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return nullptr;
    }
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_setLedgerWalletHmac(
        JNIEnv *env, jobject thiz, jstring wallet_id, jstring hmac) {
    try {
        return NunchukProvider::get()->nu->SetLedgerWalletHmac(StringWrapper(env, wallet_id),
                                                               StringWrapper(env, hmac));
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return JNI_FALSE;
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return JNI_FALSE;
    }
}
