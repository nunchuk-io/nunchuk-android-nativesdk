#include <cstring>
#include <jni.h>
#include <syslog.h>
#include <nunchuk.h>
#include <nunchukmatrix.h>
#include "string-wrapper.h"
#include "serializer.h"
#include "utils/ndef.hpp"

using namespace nunchuk;
using namespace nunchuk::ndef;

WalletType Serializer::convert2CWalletType(jint ordinal) {
    syslog(LOG_DEBUG, "[JNI][Serializer::convert2CWalletType]ordinal:: %d", ordinal);
    WalletType type;
    switch (ordinal) {
        case 0:
            type = WalletType::SINGLE_SIG;
            break;
        case 1:
            type = WalletType::MULTI_SIG;
            break;
        default:
            type = WalletType::ESCROW;
            break;
    }
    return type;
}

AddressType Serializer::convert2CAddressType(jint ordinal) {
    syslog(LOG_DEBUG, "[JNI][Serializer::convert2CAddressType]ordinal:: %d", ordinal);
    AddressType type;
    switch (ordinal) {
        case 0:
            type = AddressType::ANY;
            break;
        case 1:
            type = AddressType::LEGACY;
            break;
        case 2:
            type = AddressType::NESTED_SEGWIT;
            break;
        case 3:
            type = AddressType::NATIVE_SEGWIT;
            break;
        case 4:
            type = AddressType::TAPROOT;
            break;
        default:
            type = AddressType::NATIVE_SEGWIT;
            break;
    }
    syslog(LOG_DEBUG, "[JNI][Serializer::convert2CAddressType]type:: %d", type);
    return type;
}

SignerType Serializer::convert2CSignerType(JNIEnv *env, jobject singerType) {
    jclass clazz = env->FindClass("com/nunchuk/android/type/SignerType");
    jmethodID method = env->GetMethodID(clazz, "ordinal", "()I");
    jint ordinal = env->CallIntMethod(singerType, method);
    syslog(LOG_DEBUG, "[JNI][Serializer::convert2CSignerType]ordinal:: %d", ordinal);
    SignerType type;
    switch (ordinal) {
        case 1:
            type = SignerType::HARDWARE;
            break;
        case 2:
            type = SignerType::AIRGAP;
            break;
        case 3:
            type = SignerType::SOFTWARE;
            break;
        case 4:
            type = SignerType::FOREIGN_SOFTWARE;
            break;
        case 5:
            type = SignerType::NFC;
            break;
        case 6:
            type = SignerType::COLDCARD_NFC;
            break;
        case 7:
            type = SignerType::SERVER;
            break;
        default:
            type = SignerType::UNKNOWN;
            break;
    }
    syslog(LOG_DEBUG, "[JNI][Serializer::convert2CSignerType]type:: %d", type);
    return type;
}

SignerTag Serializer::convert2CSignerTag(JNIEnv *env, jobject tag) {
    jclass clazz = env->FindClass("com/nunchuk/android/type/SignerTag");
    jmethodID method = env->GetMethodID(clazz, "ordinal", "()I");
    jint ordinal = env->CallIntMethod(tag, method);
    syslog(LOG_DEBUG, "[JNI][Serializer::convert2CSignerTag]ordinal:: %d", ordinal);
    SignerTag type;
    switch (ordinal) {
        case 0:
            type = SignerTag::INHERITANCE;
            break;
        case 1:
            type = SignerTag::KEYSTONE;
            break;
        case 2:
            type = SignerTag::JADE;
            break;
        case 3:
            type = SignerTag::PASSPORT;
            break;
        case 4:
            type = SignerTag::SEEDSIGNER;
            break;
        case 5:
            type = SignerTag::COLDCARD;
            break;
        case 6:
            type = SignerTag::TREZOR;
            break;
        case 7:
            type = SignerTag::LEDGER;
            break;
        case 8:
            type = SignerTag::BITBOX;
            break;
        default:
            type = SignerTag::INHERITANCE;
            break;
    }
    syslog(LOG_DEBUG, "[JNI][Serializer::convert2CSignerTag]type:: %d", type);
    return type;
}

std::vector<SignerTag> Serializer::convert2CSignerTags(JNIEnv *env, jobject tags) {
    jclass cList = env->FindClass("java/util/List");

    jmethodID sizeMethod = env->GetMethodID(cList, "size", "()I");
    jmethodID getMethod = env->GetMethodID(cList, "get", "(I)Ljava/lang/Object;");

    jint size = env->CallIntMethod(tags, sizeMethod);
    std::vector<SignerTag> result;
    for (jint i = 0; i < size; i++) {
        auto item = (jobject) env->CallObjectMethod(tags, getMethod, i);
        const SignerTag singleSigner = Serializer::convert2CSignerTag(env, item);
        result.push_back(singleSigner);
    }
    return result;
}

Chain Serializer::convert2CChain(jint ordinal) {
    syslog(LOG_DEBUG, "[JNI][Serializer::convert2CChain]ordinal:: %d", ordinal);
    Chain chain;
    switch (ordinal) {
        case 0:
            chain = Chain::MAIN;
            break;
        case 1:
            chain = Chain::TESTNET;
            break;
        case 2:
            chain = Chain::SIGNET;
            break;
        default:
            chain = Chain::REGTEST;
            break;
    }
    return chain;
}

BackendType Serializer::convert2CBackendType(jint ordinal) {
    syslog(LOG_DEBUG, "[JNI][Serializer::convert2CBackendType]ordinal:: %d", ordinal);
    return ordinal == 0 ? BackendType::ELECTRUM : BackendType::CORERPC;
}

ExportFormat Serializer::convert2CExportFormat(jint ordinal) {
    syslog(LOG_DEBUG, "[JNI][Serializer::convert2CExportFormat]ordinal:: %d", ordinal);
    ExportFormat format;
    switch (ordinal) {
        case 0:
            format = ExportFormat::DB;
            break;
        case 1:
            format = ExportFormat::DESCRIPTOR;
            break;
        case 2:
            format = ExportFormat::COLDCARD;
            break;
        case 3:
            format = ExportFormat::COBO;
            break;
        case 4:
            format = ExportFormat::CSV;
            break;
        default:
            format = ExportFormat::BSMS;
            break;
    }
    return format;
}

std::vector<std::string> Serializer::convert2CListString(JNIEnv *env, jobject values) {
    jclass cList = env->FindClass("java/util/List");

    jmethodID sizeMethod = env->GetMethodID(cList, "size", "()I");
    jmethodID getMethod = env->GetMethodID(cList, "get", "(I)Ljava/lang/Object;");

    jint size = env->CallIntMethod(values, sizeMethod);
    std::vector<std::string> result;
    for (jint i = 0; i < size; i++) {
        auto item = (jstring) env->CallObjectMethod(values, getMethod, i);
        auto value = env->GetStringUTFChars(item, JNI_FALSE);
        result.emplace_back(value);
    }
    return result;
}

SingleSigner Serializer::convert2CSigner(JNIEnv *env, jobject signer) {
    jclass clazz = env->FindClass("com/nunchuk/android/model/SingleSigner");

    jfieldID fieldName = env->GetFieldID(clazz, "name", "Ljava/lang/String;");
    auto nameVal = (jstring) env->GetObjectField(signer, fieldName);
    const char *name = env->GetStringUTFChars(nameVal, nullptr);

    jfieldID fieldXpub = env->GetFieldID(clazz, "xpub", "Ljava/lang/String;");
    auto xpubVal = (jstring) env->GetObjectField(signer, fieldXpub);
    const char *xpub = env->GetStringUTFChars(xpubVal, nullptr);

    jfieldID fieldPublicKey = env->GetFieldID(clazz, "publicKey", "Ljava/lang/String;");
    auto publicKeyVal = (jstring) env->GetObjectField(signer, fieldPublicKey);
    const char *public_key = env->GetStringUTFChars(publicKeyVal, nullptr);

    jfieldID fieldDerivationPath = env->GetFieldID(clazz, "derivationPath", "Ljava/lang/String;");
    auto derivationPathVal = (jstring) env->GetObjectField(signer, fieldDerivationPath);
    const char *derivation_path = env->GetStringUTFChars(derivationPathVal, nullptr);

    jfieldID fieldMasterFingerprint = env->GetFieldID(clazz, "masterFingerprint", "Ljava/lang/String;");
    auto masterFingerprintVal = (jstring) env->GetObjectField(signer, fieldMasterFingerprint);
    const char *master_fingerprint = env->GetStringUTFChars(masterFingerprintVal, nullptr);

    jfieldID fieldSignerType = env->GetFieldID(clazz, "type", "Lcom/nunchuk/android/type/SignerType;");
    jobject fieldSignerTypeVal = (jobject) env->GetObjectField(signer, fieldSignerType);
    const SignerType &signer_type = convert2CSignerType(env, fieldSignerTypeVal);

    jfieldID fieldVisible = env->GetFieldID(clazz, "isVisible", "Z");
    auto isVisible = env->GetBooleanField(signer, fieldVisible);

    auto singleSigner = SingleSigner(name, xpub, public_key, derivation_path, master_fingerprint, 0);
    singleSigner.set_type(signer_type);
    jfieldID fieldTagsId = env->GetFieldID(clazz, "tags", "Ljava/util/List;");
    auto tags = (jobject) env->GetObjectField(signer, fieldTagsId);
    singleSigner.set_tags(Serializer::convert2CSignerTags(env, tags));
    singleSigner.set_visible(isVisible);
    syslog(LOG_DEBUG, "[JNI][SingleSigner]name:: %s", singleSigner.get_name().c_str());
    syslog(LOG_DEBUG, "[JNI][SingleSigner]xpub:: %s", singleSigner.get_xpub().c_str());
    syslog(LOG_DEBUG, "[JNI][SingleSigner]path:: %s", singleSigner.get_derivation_path().c_str());
    syslog(LOG_DEBUG, "[JNI][SingleSigner]fingerPrint:: %s", singleSigner.get_master_fingerprint().c_str());
    syslog(LOG_DEBUG, "[JNI][SingleSigner]type:: %d", singleSigner.get_type());

    env->ReleaseStringUTFChars(nameVal, name);
    env->ReleaseStringUTFChars(xpubVal, xpub);
    env->ReleaseStringUTFChars(publicKeyVal, public_key);
    env->ReleaseStringUTFChars(derivationPathVal, derivation_path);
    env->ReleaseStringUTFChars(masterFingerprintVal, master_fingerprint);
    return singleSigner;
}

MasterSigner Serializer::convert2CMasterSigner(JNIEnv *env, jobject signer) {
    jclass clazz = env->FindClass("com/nunchuk/android/model/MasterSigner");

    jfieldID fieldId = env->GetFieldID(clazz, "id", "Ljava/lang/String;");
    auto idVal = (jstring) env->GetObjectField(signer, fieldId);
    const char *id = env->GetStringUTFChars(idVal, JNI_FALSE);

    jfieldID fieldName = env->GetFieldID(clazz, "name", "Ljava/lang/String;");
    auto nameVal = (jstring) env->GetObjectField(signer, fieldName);
    const char *name = env->GetStringUTFChars(nameVal, JNI_FALSE);

    jfieldID fieldLastHealthCheck = env->GetFieldID(clazz, "lastHealthCheck", "J");
    auto last_health_check = env->GetLongField(signer, fieldLastHealthCheck);

    jfieldID fieldSoftware = env->GetFieldID(clazz, "software", "Z");
    auto software = env->GetBooleanField(signer, fieldSoftware);

    jfieldID fieldDevice = env->GetFieldID(clazz, "device", "Lcom/nunchuk/android/model/Device;");
    auto deviceVal = (jobject) env->GetObjectField(signer, fieldDevice);
    auto device = convert2CDevice(env, deviceVal);

    jfieldID fieldSignerType = env->GetFieldID(clazz, "type", "Lcom/nunchuk/android/type/SignerType;");
    jobject fieldSignerTypeVal = (jobject) env->GetObjectField(signer, fieldSignerType);
    SignerType signerType;
    if (software) {
        signerType = SignerType::SOFTWARE;
    } else {
        signerType = convert2CSignerType(env, fieldSignerTypeVal);
    }

    MasterSigner masterSigner = MasterSigner(id, device, last_health_check, signerType);
    masterSigner.set_name(name);
    jfieldID fieldTagsId = env->GetFieldID(clazz, "tags", "Ljava/util/List;");
    auto tags = (jobject) env->GetObjectField(signer, fieldTagsId);
    masterSigner.set_tags(Serializer::convert2CSignerTags(env, tags));
    jfieldID fieldVisible = env->GetFieldID(clazz, "isVisible", "Z");
    auto isVisible = env->GetBooleanField(signer, fieldVisible);
    masterSigner.set_visible(isVisible);
    syslog(LOG_DEBUG, "[JNI][MasterSigner]id:: %s", masterSigner.get_id().c_str());
    syslog(LOG_DEBUG, "[JNI][MasterSigner]name:: %s", masterSigner.get_name().c_str());
    syslog(LOG_DEBUG, "[JNI][MasterSigner]path:: %s", masterSigner.get_device().get_path().c_str());
    syslog(LOG_DEBUG, "[JNI][MasterSigner]fingerPrint:: %s", masterSigner.get_device().get_master_fingerprint().c_str());

    env->ReleaseStringUTFChars(idVal, id);
    env->ReleaseStringUTFChars(nameVal, name);
    return masterSigner;
}

Device Serializer::convert2CDevice(JNIEnv *env, jobject device) {
    jclass clazz = env->FindClass("com/nunchuk/android/model/Device");

    jfieldID fieldType = env->GetFieldID(clazz, "type", "Ljava/lang/String;");
    auto typeVal = (jstring) env->GetObjectField(device, fieldType);
    const char *type = env->GetStringUTFChars(typeVal, JNI_FALSE);

    jfieldID fieldModel = env->GetFieldID(clazz, "model", "Ljava/lang/String;");
    auto modelVal = (jstring) env->GetObjectField(device, fieldModel);
    const char *model = env->GetStringUTFChars(modelVal, JNI_FALSE);

    jfieldID fieldPath = env->GetFieldID(clazz, "path", "Ljava/lang/String;");
    auto pathVal = (jstring) env->GetObjectField(device, fieldPath);
    const char *path = env->GetStringUTFChars(pathVal, JNI_FALSE);

    jfieldID fieldMasterFingerprint = env->GetFieldID(clazz, "masterFingerprint", "Ljava/lang/String;");
    auto masterFingerprintVal = (jstring) env->GetObjectField(device, fieldMasterFingerprint);
    const char *master_fingerprint = env->GetStringUTFChars(masterFingerprintVal, JNI_FALSE);

    jfieldID fieldConnected = env->GetFieldID(clazz, "connected", "Z");
    auto connected = env->GetBooleanField(device, fieldConnected);

    jfieldID fieldNeedPassPhraseSent = env->GetFieldID(clazz, "needPassPhraseSent", "Z");
    auto needs_pass_phrase_sent = env->GetBooleanField(device, fieldNeedPassPhraseSent);

    jfieldID fieldNeedPinSet = env->GetFieldID(clazz, "needPinSet", "Z");
    auto need_pin_set = env->GetBooleanField(device, fieldNeedPinSet);

    const Device &device_ = Device(type, path, model, master_fingerprint, connected, needs_pass_phrase_sent, need_pin_set);
    syslog(LOG_DEBUG, "[JNI][Device]type:: %s", device_.get_type().c_str());
    syslog(LOG_DEBUG, "[JNI][Device]model:: %s", device_.get_model().c_str());
    syslog(LOG_DEBUG, "[JNI][Device]path:: %s", device_.get_path().c_str());
    syslog(LOG_DEBUG, "[JNI][Device]fingerPrint:: %s", device_.get_master_fingerprint().c_str());

    env->ReleaseStringUTFChars(typeVal, type);
    env->ReleaseStringUTFChars(modelVal, model);
    env->ReleaseStringUTFChars(pathVal, path);
    env->ReleaseStringUTFChars(masterFingerprintVal, master_fingerprint);
    return device_;
}

std::vector<SingleSigner> Serializer::convert2CSigners(JNIEnv *env, jobject signers) {
    jclass cList = env->FindClass("java/util/List");

    jmethodID sizeMethod = env->GetMethodID(cList, "size", "()I");
    jmethodID getMethod = env->GetMethodID(cList, "get", "(I)Ljava/lang/Object;");

    jint size = env->CallIntMethod(signers, sizeMethod);
    std::vector<SingleSigner> result;
    for (jint i = 0; i < size; i++) {
        auto item = (jobject) env->CallObjectMethod(signers, getMethod, i);
        const SingleSigner singleSigner = Serializer::convert2CSigner(env, item);
        result.push_back(singleSigner);
    }
    return result;
}

Wallet Serializer::convert2CWallet(JNIEnv *env, jobject wallet) {
    jclass clazz = env->FindClass("com/nunchuk/android/model/bridge/WalletBridge");

    jfieldID fieldId = env->GetFieldID(clazz, "id", "Ljava/lang/String;");
    auto idVal = (jstring) env->GetObjectField(wallet, fieldId);
    const char *id = env->GetStringUTFChars(idVal, nullptr);

    jfieldID fieldName = env->GetFieldID(clazz, "name", "Ljava/lang/String;");
    auto nameVal = (jstring) env->GetObjectField(wallet, fieldName);
    const char *name = env->GetStringUTFChars(nameVal, nullptr);

    jfieldID fieldTotalRequireSigns = env->GetFieldID(clazz, "totalRequireSigns", "I");
    auto total_required_signs = (jint) env->GetIntField(wallet, fieldTotalRequireSigns);

    jfieldID fieldSigners = env->GetFieldID(clazz, "signers", "Ljava/util/List;");
    auto signersVal = (jobject) env->GetObjectField(wallet, fieldSigners);
    auto signers = Serializer::convert2CSigners(env, signersVal);

    jfieldID fieldAddressType = env->GetFieldID(clazz, "addressType", "I");
    jint addressTypeVal = env->GetIntField(wallet, fieldAddressType);
    auto address_type = Serializer::convert2CAddressType(addressTypeVal);

    jfieldID fieldEscrow = env->GetFieldID(clazz, "escrow", "Z");
    auto escrow = env->GetBooleanField(wallet, fieldEscrow);

    jfieldID fieldGapLimit = env->GetFieldID(clazz, "gapLimit", "I");
    auto gap_limit = (jint) env->GetIntField(wallet, fieldGapLimit);

    Wallet updateWallet = Wallet(id, total_required_signs, signers.size(), signers, address_type, escrow, 0);
    updateWallet.set_name(name);
    updateWallet.set_gap_limit(gap_limit);

    env->ReleaseStringUTFChars(nameVal, name);
    env->ReleaseStringUTFChars(idVal, id);

    return updateWallet;
}

TxInput Serializer::convert2CTxInput(JNIEnv *env, jobject input) {
    jclass clazz = env->FindClass("com/nunchuk/android/model/TxInput");

    jfieldID fieldFirst = env->GetFieldID(clazz, "first", "Ljava/lang/String;");
    auto firstVal = (jstring) env->GetObjectField(input, fieldFirst);
    auto first = env->GetStringUTFChars(firstVal, JNI_FALSE);

    jfieldID fieldSecond = env->GetFieldID(clazz, "second", "I");
    auto secondVal = env->GetIntField(input, fieldSecond);

    env->ReleaseStringUTFChars(firstVal, first);
    return TxInput(first, secondVal);
}

std::vector<TxInput> Serializer::convert2CTxInputs(JNIEnv *env, jobject inputs) {
    jclass cList = env->FindClass("java/util/List");

    jmethodID sizeMethod = env->GetMethodID(cList, "size", "()I");
    jmethodID getMethod = env->GetMethodID(cList, "get", "(I)Ljava/lang/Object;");

    jint size = env->CallIntMethod(inputs, sizeMethod);
    std::vector<TxInput> result;
    for (jint i = 0; i < size; i++) {
        auto _item = (jobject) env->CallObjectMethod(inputs, getMethod, i);
        auto item = Serializer::convert2CTxInput(env, _item);
        result.push_back(item);
    }
    return result;
}

TxOutput Serializer::convert2CTxOutput(JNIEnv *env, jobject input) {
    jclass clazz = env->FindClass("com/nunchuk/android/model/TxOutput");

    jfieldID fieldFirst = env->GetFieldID(clazz, "first", "Ljava/lang/String;");
    auto firstVal = (jstring) env->GetObjectField(input, fieldFirst);
    auto first = env->GetStringUTFChars(firstVal, JNI_FALSE);

    jfieldID fieldSecond = env->GetFieldID(clazz, "second", "Lcom/nunchuk/android/model/Amount;");
    auto secondVal = env->GetObjectField(input, fieldSecond);

    env->ReleaseStringUTFChars(firstVal, first);

    return TxOutput(first, Serializer::convert2CAmount(env, secondVal));
}

std::vector<TxOutput> Serializer::convert2CTxOutputs(JNIEnv *env, jobject outputs) {
    jclass cList = env->FindClass("java/util/List");

    jmethodID sizeMethod = env->GetMethodID(cList, "size", "()I");
    jmethodID getMethod = env->GetMethodID(cList, "get", "(I)Ljava/lang/Object;");

    jint size = env->CallIntMethod(outputs, sizeMethod);
    std::vector<TxOutput> result;
    for (jint i = 0; i < size; i++) {
        auto _item = (jobject) env->CallObjectMethod(outputs, getMethod, i);
        auto item = Serializer::convert2CTxOutput(env, _item);
        result.push_back(item);
    }
    return result;
}

UnspentOutput Serializer::convert2CUnspentOutput(JNIEnv *env, jobject unspentOutput) {
    jclass clazz = env->FindClass("com/nunchuk/android/model/UnspentOutput");

    jfieldID fieldTxId = env->GetFieldID(clazz, "txid", "Ljava/lang/String;");
    auto txIdVal = (jstring) env->GetObjectField(unspentOutput, fieldTxId);
    auto txId = StringWrapper(env, txIdVal);

    jfieldID fieldAddress = env->GetFieldID(clazz, "address", "Ljava/lang/String;");
    auto addressVal = (jstring) env->GetObjectField(unspentOutput, fieldAddress);
    auto address = StringWrapper(env, addressVal);

    jfieldID fieldVOut = env->GetFieldID(clazz, "vout", "I");
    auto vout = env->GetIntField(unspentOutput, fieldVOut);

    jfieldID fieldAmount = env->GetFieldID(clazz, "amount", "Lcom/nunchuk/android/model/Amount;");
    auto amountVal = env->GetObjectField(unspentOutput, fieldAmount);

    jfieldID fieldHeight = env->GetFieldID(clazz, "height", "I");
    auto height = env->GetIntField(unspentOutput, fieldHeight);

    jfieldID fieldMemo = env->GetFieldID(clazz, "memo", "Ljava/lang/String;");
    auto memoVal = (jstring) env->GetObjectField(unspentOutput, fieldMemo);
    auto memo = StringWrapper(env, memoVal);

    UnspentOutput output = UnspentOutput();
    output.set_txid(txId);
    output.set_vout(vout);
    output.set_address(address);
    output.set_amount(Serializer::convert2CAmount(env, amountVal));
    output.set_height(height);
    output.set_memo(memo);
    return output;
}

std::vector<UnspentOutput> Serializer::convert2CUnspentOutputs(JNIEnv *env, jobject unspentOutputs) {
    jclass cList = env->FindClass("java/util/List");

    jmethodID sizeMethod = env->GetMethodID(cList, "size", "()I");
    jmethodID getMethod = env->GetMethodID(cList, "get", "(I)Ljava/lang/Object;");

    jint size = env->CallIntMethod(unspentOutputs, sizeMethod);
    std::vector<UnspentOutput> result;
    for (jint i = 0; i < size; i++) {
        auto _item = (jobject) env->CallObjectMethod(unspentOutputs, getMethod, i);
        auto item = Serializer::convert2CUnspentOutput(env, _item);
        result.push_back(item);
    }
    return result;
}

std::map<std::string, Amount> Serializer::convert2CAmountsMap(JNIEnv *env, jobject outputs) {
    std::map<std::string, Amount> mapOut = std::map<std::string, Amount>();

    // Get the Map's entry Set.
    jclass mapClass = env->FindClass("java/util/Map");
    jmethodID entrySet = env->GetMethodID(mapClass, "entrySet", "()Ljava/util/Set;");
    jobject set = env->CallObjectMethod(outputs, entrySet);
    jclass setClass = env->FindClass("java/util/Set");
    jmethodID iteratorMethodId = env->GetMethodID(setClass, "iterator", "()Ljava/util/Iterator;");
    jobject iterator = env->CallObjectMethod(set, iteratorMethodId);

    // Get the Iterator method IDs
    jclass iteratorClass = env->FindClass("java/util/Iterator");
    jmethodID hasNext = env->GetMethodID(iteratorClass, "hasNext", "()Z");
    jmethodID next = env->GetMethodID(iteratorClass, "next", "()Ljava/lang/Object;");

    // Get the Entry class method IDs
    jclass entryClass = env->FindClass("java/util/Map$Entry");
    jmethodID getKey = env->GetMethodID(entryClass, "getKey", "()Ljava/lang/Object;");
    jmethodID getValue = env->GetMethodID(entryClass, "getValue", "()Ljava/lang/Object;");

    // Iterate over the entry Set
    while (env->CallBooleanMethod(iterator, hasNext)) {
        jobject entry = env->CallObjectMethod(iterator, next);
        auto key = (jstring) env->CallObjectMethod(entry, getKey);
        auto value = (jobject) env->CallObjectMethod(entry, getValue);
        const char *keyStr = env->GetStringUTFChars(key, JNI_FALSE);
        Amount valueStr = Serializer::convert2CAmount(env, value);

        mapOut.insert(std::make_pair(std::string(keyStr), valueStr));

    }
    return mapOut;
}

Amount Serializer::convert2CAmount(JNIEnv *env, jobject amount) {
    jclass clazz = env->FindClass("com/nunchuk/android/model/Amount");
    jfieldID fieldId = env->GetFieldID(clazz, "value", "J");
    return env->GetLongField(amount, fieldId);
}

NunchukMatrixEvent Serializer::convert2CMatrixEvent(JNIEnv *env, jobject event) {
    jclass clazz = env->FindClass("com/nunchuk/android/model/NunchukMatrixEvent");

    jfieldID fieldType = env->GetFieldID(clazz, "type", "Ljava/lang/String;");
    auto typeVal = (jstring) env->GetObjectField(event, fieldType);
    auto type = env->GetStringUTFChars(typeVal, JNI_FALSE);

    jfieldID fieldContent = env->GetFieldID(clazz, "content", "Ljava/lang/String;");
    auto contentVal = (jstring) env->GetObjectField(event, fieldContent);
    auto content = env->GetStringUTFChars(contentVal, JNI_FALSE);

    jfieldID fieldEventId = env->GetFieldID(clazz, "eventId", "Ljava/lang/String;");
    auto eventIdVal = (jstring) env->GetObjectField(event, fieldEventId);
    auto eventId = env->GetStringUTFChars(eventIdVal, JNI_FALSE);

    jfieldID fieldRoomId = env->GetFieldID(clazz, "roomId", "Ljava/lang/String;");
    auto roomIdVal = (jstring) env->GetObjectField(event, fieldRoomId);
    auto roomId = env->GetStringUTFChars(roomIdVal, JNI_FALSE);

    jfieldID fieldSender = env->GetFieldID(clazz, "sender", "Ljava/lang/String;");
    auto senderVal = (jstring) env->GetObjectField(event, fieldSender);
    auto sender = env->GetStringUTFChars(senderVal, JNI_FALSE);

    jfieldID fieldTime = env->GetFieldID(clazz, "time", "J");
    auto time = env->GetLongField(event, fieldTime);

    NunchukMatrixEvent matrixEvent = NunchukMatrixEvent();
    matrixEvent.set_type(type);
    matrixEvent.set_content(content);
    matrixEvent.set_event_id(eventId);
    matrixEvent.set_room_id(roomId);
    matrixEvent.set_sender(sender);
    matrixEvent.set_ts(time);

    syslog(LOG_DEBUG, "[JNI][NunchukMatrixEvent]type:: %s", matrixEvent.get_type().c_str());
    syslog(LOG_DEBUG, "[JNI][NunchukMatrixEvent]content:: %s", matrixEvent.get_content().c_str());
    syslog(LOG_DEBUG, "[JNI][NunchukMatrixEvent]eventId:: %s", matrixEvent.get_event_id().c_str());
    syslog(LOG_DEBUG, "[JNI][NunchukMatrixEvent]roomId:: %s", matrixEvent.get_room_id().c_str());
    syslog(LOG_DEBUG, "[JNI][NunchukMatrixEvent]sender:: %s", matrixEvent.get_sender().c_str());
    syslog(LOG_DEBUG, "[JNI][NunchukMatrixEvent]time:: %ld", matrixEvent.get_ts());

    env->ReleaseStringUTFChars(typeVal, type);
    env->ReleaseStringUTFChars(contentVal, content);
    env->ReleaseStringUTFChars(eventIdVal, eventId);
    env->ReleaseStringUTFChars(roomIdVal, roomId);
    env->ReleaseStringUTFChars(senderVal, sender);

    return matrixEvent;
}

SatscardSlot::Status Serializer::convert2CSatsCardSlotStatus(JNIEnv *env, jobject satsCardSlotStatus) {
    jclass clazz = env->FindClass("com/nunchuk/android/type/SatsCardSlotStatus");
    jmethodID method = env->GetMethodID(clazz, "ordinal", "()I");
    jint ordinal = env->CallIntMethod(satsCardSlotStatus, method);
    syslog(LOG_DEBUG, "[JNI][Serializer::convert2CSatsCardSlotStatus]ordinal:: %d", ordinal);
    switch (ordinal) {
        case 0:
            return SatscardSlot::Status::UNUSED;
        case 1:
            return SatscardSlot::Status::SEALED;
        default:
            return SatscardSlot::Status::UNSEALED;
    }
}

SatscardSlot Serializer::convert2CSatsCardSlot(JNIEnv *env, jobject slot) {
    jclass clazz = env->FindClass("com/nunchuk/android/model/SatsCardSlot");

    jfieldID fieldStatus = env->GetFieldID(clazz, "status", "Lcom/nunchuk/android/type/SatsCardSlotStatus;");
    auto statusVal = (jobject) env->GetObjectField(slot, fieldStatus);
    auto status = convert2CSatsCardSlotStatus(env, statusVal);

    jfieldID fieldAddress = env->GetFieldID(clazz, "address", "Ljava/lang/String;");
    auto addressVal = (jstring) env->GetObjectField(slot, fieldAddress);
    auto address = env->GetStringUTFChars(addressVal, JNI_FALSE);

    jfieldID fieldIndex = env->GetFieldID(clazz, "index", "I");
    auto index = env->GetIntField(slot, fieldIndex);

    jfieldID fieldBalance = env->GetFieldID(clazz, "balance", "Lcom/nunchuk/android/model/Amount;");
    auto balanceVal = (jobject) env->GetObjectField(slot, fieldBalance);
    auto balance = convert2CAmount(env, balanceVal);

    jfieldID fieldConfirmed = env->GetFieldID(clazz, "isConfirmed", "Z");
    auto isConfirmed = env->GetBooleanField(slot, fieldConfirmed);

    jfieldID fieldOutputs = env->GetFieldID(clazz, "outputs", "Ljava/util/List;");
    auto outputsVal = (jobject) env->GetObjectField(slot, fieldOutputs);
    auto outputs = Serializer::convert2CUnspentOutputs(env, outputsVal);

    auto publicKeyFieldID = env->GetFieldID(clazz, "publicKey", "[B");
    auto publicKeyByteArray = (jbyteArray) env->GetObjectField(slot, publicKeyFieldID);
    auto publicKeyVal = convert2CByteArray(env, publicKeyByteArray);

    auto privateKeyFieldID = env->GetFieldID(clazz, "privateKey", "[B");
    auto privateKeyByteArray = (jbyteArray) env->GetObjectField(slot, privateKeyFieldID);
    auto firstBytePrivateKey = env->GetByteArrayElements(privateKeyByteArray, JNI_FALSE);
    std::vector<unsigned char> privateKeyVal((char *) firstBytePrivateKey,
                                             (char *) firstBytePrivateKey + env->GetArrayLength(privateKeyByteArray));
    env->ReleaseByteArrayElements(privateKeyByteArray, firstBytePrivateKey, JNI_ABORT);

    auto chainCodeFieldID = env->GetFieldID(clazz, "chainCode", "[B");
    auto chainCodeArray = (jbyteArray) env->GetObjectField(slot, chainCodeFieldID);
    auto firstByteChainCode = env->GetByteArrayElements(chainCodeArray, JNI_FALSE);
    std::vector<unsigned char> chainCodeVal((char *) firstByteChainCode,
                                            (char *) firstByteChainCode + env->GetArrayLength(chainCodeArray));
    env->ReleaseByteArrayElements(chainCodeArray, firstByteChainCode, JNI_ABORT);

    auto masterPrivateKeyFieldID = env->GetFieldID(clazz, "masterPrivateKey", "[B");
    auto masterPrivateKeyArray = (jbyteArray) env->GetObjectField(slot, masterPrivateKeyFieldID);
    auto firstByteMasterPrivateKey = env->GetByteArrayElements(masterPrivateKeyArray, JNI_FALSE);
    std::vector<unsigned char> masterPrivateKeyVal((char *) firstByteMasterPrivateKey,
                                                   (char *) firstByteMasterPrivateKey + env->GetArrayLength(masterPrivateKeyArray));
    env->ReleaseByteArrayElements(masterPrivateKeyArray, firstByteMasterPrivateKey, JNI_ABORT);

    auto satsCardSlot = SatscardSlot();
    satsCardSlot.set_status(status);
    satsCardSlot.set_address(address);
    satsCardSlot.set_index(index);
    satsCardSlot.set_balance(balance);
    satsCardSlot.set_confirmed(isConfirmed);
    satsCardSlot.set_utxos(outputs);
    satsCardSlot.set_pubkey(publicKeyVal);
    satsCardSlot.set_privkey(privateKeyVal);
    satsCardSlot.set_chain_code(chainCodeVal);
    satsCardSlot.set_master_privkey(masterPrivateKeyVal);

    return satsCardSlot;
}

std::vector<SatscardSlot> Serializer::convert2CSatsCardSlots(JNIEnv *env, jobject slots) {
    jclass cList = env->FindClass("java/util/List");

    jmethodID sizeMethod = env->GetMethodID(cList, "size", "()I");
    jmethodID getMethod = env->GetMethodID(cList, "get", "(I)Ljava/lang/Object;");

    jint size = env->CallIntMethod(slots, sizeMethod);
    std::vector<SatscardSlot> result;
    for (jint i = 0; i < size; i++) {
        auto _item = (jobject) env->CallObjectMethod(slots, getMethod, i);
        auto item = Serializer::convert2CSatsCardSlot(env, _item);
        result.push_back(item);
    }
    return result;
}

std::vector<unsigned char> Serializer::convert2CByteArray(JNIEnv *env, jbyteArray byteArray) {
    auto element = env->GetByteArrayElements(byteArray, nullptr);
    std::vector<unsigned char> ret(element, element + env->GetArrayLength(byteArray));
    env->ReleaseByteArrayElements(byteArray, element, JNI_ABORT);
    return ret;
}

std::vector<NDEFRecord> Serializer::convert2CRecords(JNIEnv *env, jobjectArray records) {
    static auto NdefRecordClass = (jclass) env->NewGlobalRef(
            env->FindClass("android/nfc/NdefRecord"));
    int length = env->GetArrayLength(records);
    std::vector<NDEFRecord> ret;
    ret.reserve(length);

    for (int i = 0; i < length; ++i) {
        auto record = env->GetObjectArrayElement(records, i);
        auto id = (jbyteArray) env->CallObjectMethod(
                record,
                env->GetMethodID(NdefRecordClass, "getId", "()[B")
        );
        auto payload = (jbyteArray) env->CallObjectMethod(
                record,
                env->GetMethodID(NdefRecordClass,
                                 "getPayload", "()[B")
        );
        auto tnf = env->CallShortMethod(
                record,
                env->GetMethodID(NdefRecordClass, "getTnf", "()S")
        );
        auto type = (jbyteArray) env->CallObjectMethod(
                record,
                env->GetMethodID(NdefRecordClass, "getType", "()[B")
        );
        ret.push_back(NDEFRecord{
                static_cast<unsigned char>(tnf),
                convert2CByteArray(env, type),
                convert2CByteArray(env, id),
                convert2CByteArray(env, payload),
        });
    }
    return ret;
}

CoinTag Serializer::convert2CCoinTag(JNIEnv *env, jobject tag) {
    jclass clazz = env->FindClass("com/nunchuk/android/model/CoinTag");

    jfieldID fieldId = env->GetFieldID(clazz, "id", "I");
    auto id = env->GetIntField(tag, fieldId);

    jfieldID fieldName = env->GetFieldID(clazz, "name", "Ljava/lang/String;");
    auto nameVal = (jstring) env->GetObjectField(tag, fieldName);
    auto name = env->GetStringUTFChars(nameVal, JNI_FALSE);

    jfieldID fieldColor = env->GetFieldID(clazz, "color", "Ljava/lang/String;");
    auto colorVal = (jstring) env->GetObjectField(tag, fieldColor);
    auto color = env->GetStringUTFChars(colorVal, JNI_FALSE);

    env->ReleaseStringUTFChars(nameVal, name);
    env->ReleaseStringUTFChars(colorVal, color);

    return CoinTag(id, name, color);
}

CoinCollection Serializer::convert2CCoinCollection(JNIEnv *env, jobject collection) {
    jclass clazz = env->FindClass("com/nunchuk/android/model/CoinCollection");

    jfieldID fieldId = env->GetFieldID(clazz, "id", "I");
    auto id = env->GetIntField(collection, fieldId);

    jfieldID fieldName = env->GetFieldID(clazz, "name", "Ljava/lang/String;");
    auto nameVal = (jstring) env->GetObjectField(collection, fieldName);
    auto name = StringWrapper(env, nameVal);
    jfieldID fieldAddNewCoin = env->GetFieldID(clazz, "isAddNewCoin", "Z");
    auto isAddNewCoin = env->GetBooleanField(collection, fieldAddNewCoin);
    jfieldID fieldAutoLock = env->GetFieldID(clazz, "isAutoLock", "Z");
    auto isAutoLock = env->GetBooleanField(collection, fieldAutoLock);

    CoinCollection coinCollection = CoinCollection(id, name);
    coinCollection.set_add_new_coin(isAddNewCoin);
    coinCollection.set_auto_lock(isAutoLock);
    return coinCollection;
}
