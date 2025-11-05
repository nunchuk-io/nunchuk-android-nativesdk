#include <cstring>
#include <jni.h>
#include <syslog.h>
#include <nunchuk.h>
#include <iostream>
#include <sstream>
#include <string>
#include <initializer.h>
#include "deserializer.h"
#include "utils/ndef.hpp"

using namespace nunchuk;
using namespace nunchuk::ndef;

jobject Deserializer::convert2JBoolean(JNIEnv *env, const bool value) {
    jclass clazz = env->FindClass("java/lang/Boolean");
    jmethodID methodId = env->GetMethodID(clazz, "<init>", "(Z)V");
    return env->NewObject(clazz, methodId, value ? JNI_TRUE : JNI_FALSE);
}

jobject Deserializer::convert2JListString(JNIEnv *env, const std::vector<std::string> &values) {
    static auto arrayListClass = static_cast<jclass>(env->NewGlobalRef(
            env->FindClass("java/util/ArrayList")));
    static jmethodID constructor = env->GetMethodID(arrayListClass, "<init>", "()V");
    jmethodID addMethod = env->GetMethodID(arrayListClass, "add", "(Ljava/lang/Object;)Z");
    jobject arrayListInstance = env->NewObject(arrayListClass, constructor);
    for (const std::string &s: values) {
        auto element = env->NewStringUTF(s.c_str());
        env->CallBooleanMethod(arrayListInstance, addMethod, element);
        env->DeleteLocalRef(element);
    }
    return arrayListInstance;
}

jobject
Deserializer::convert2JStringBooleanMap(JNIEnv *env,
                                        const std::map<std::string, bool> &signersMap) {
    syslog(LOG_DEBUG, "[JNI] convert2JStringBooleanMap");
    jclass clazz = env->FindClass("java/util/HashMap");
    jmethodID init = env->GetMethodID(clazz, "<init>", "()V");
    jobject instance = env->NewObject(clazz, init);
    try {
        jmethodID putMethod = env->GetMethodID(clazz, "put",
                                               "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
        if (!signersMap.empty()) {
            for (const auto &it: signersMap) {
                auto element = env->NewStringUTF(it.first.c_str());
                env->CallObjectMethod(instance, putMethod, element,
                                      convert2JBoolean(env, it.second));
                env->DeleteLocalRef(element);
            }
        }
    } catch (const std::exception &e) {
        syslog(LOG_DEBUG, "[JNI] convert2JStringBooleanMap error::%s", e.what());
    }
    return instance;
}

void Deserializer::convert2JException(JNIEnv *env, const BaseException &e) {
    static jclass clazz = static_cast<jclass>(env->NewGlobalRef(
            env->FindClass("com/nunchuk/android/exception/NCNativeException")));
    syslog(LOG_DEBUG, "[JNI] convert2JException()");
    std::string msg = std::to_string(e.code()) + ":" + e.what();
    env->ThrowNew(clazz, msg.c_str());
}

void Deserializer::convertStdException2JException(JNIEnv *env, const std::exception &e) {
    static jclass clazz = static_cast<jclass>(env->NewGlobalRef(
            env->FindClass("com/nunchuk/android/exception/NCNativeException")));
    syslog(LOG_DEBUG, "[JNI] convertStdException2JException()");
    env->ThrowNew(clazz, e.what());
}

jobject Deserializer::convert2JDevice(JNIEnv *env, const Device &device) {
    syslog(LOG_DEBUG, "[JNI] convert2JDevice()");
    jclass clazz = env->FindClass("com/nunchuk/android/model/Device");
    jmethodID constructor = env->GetMethodID(clazz, "<init>", "()V");
    jobject instance = env->NewObject(clazz, constructor);
    try {
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setMasterFingerprint",
                                                       "(Ljava/lang/String;)V"),
                            env->NewStringUTF(device.get_master_fingerprint().c_str()));
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setType", "(Ljava/lang/String;)V"),
                            env->NewStringUTF(device.get_type().c_str()));
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setModel", "(Ljava/lang/String;)V"),
                            env->NewStringUTF(device.get_model().c_str()));
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setPath", "(Ljava/lang/String;)V"),
                            env->NewStringUTF(device.get_path().c_str()));
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setConnected", "(Z)V"),
                            device.connected());
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setNeedPassPhraseSent", "(Z)V"),
                            device.needs_pass_phrase_sent());
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setNeedPinSet", "(Z)V"),
                            device.needs_pin_sent());
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setInitialized", "(Z)V"),
                            device.initialized());
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setTapsigner", "(Z)V"),
                            device.is_tapsigner());
    } catch (const std::exception &e) {
        syslog(LOG_DEBUG, "[JNI] convert2JDevice error::%s", e.what());
    }
    return instance;
}

jobject Deserializer::convert2JDevices(JNIEnv *env, const std::vector<Device> &devices) {
    static auto arrayListClass = static_cast<jclass>(env->NewGlobalRef(
            env->FindClass("java/util/ArrayList")));
    static jmethodID constructor = env->GetMethodID(arrayListClass, "<init>", "()V");
    jmethodID addMethod = env->GetMethodID(arrayListClass, "add", "(Ljava/lang/Object;)Z");
    jobject arrayListInstance = env->NewObject(arrayListClass, constructor);
    for (const Device &device: devices) {
        jobject element = convert2JDevice(env, device);
        env->CallBooleanMethod(arrayListInstance, addMethod, element);
        env->DeleteLocalRef(element);
    }
    return arrayListInstance;
}

jobject Deserializer::convert2JAmount(JNIEnv *env, const Amount amount) {
    syslog(LOG_DEBUG, "[JNI] convert2JAmount()");
    jclass clazz = env->FindClass("com/nunchuk/android/model/Amount");
    jmethodID constructor = env->GetMethodID(clazz, "<init>", "()V");
    jobject instance = env->NewObject(clazz, constructor);
    try {
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setValue", "(J)V"),
                            (int64_t) amount);
        env->CallVoidMethod(instance,
                            env->GetMethodID(clazz, "setFormattedValue", "(Ljava/lang/String;)V"),
                            env->NewStringUTF(Utils::ValueFromAmount(amount).c_str()));
    } catch (const std::exception &e) {
        syslog(LOG_DEBUG, "[JNI] convert2JAmount error::%s", e.what());
    }
    return instance;
}

jobject Deserializer::convert2JTxInput(JNIEnv *env, const TxInput &input) {
    syslog(LOG_DEBUG, "[JNI] convert2JTxInput()");
    jclass clazz = env->FindClass("com/nunchuk/android/model/TxInput");
    jmethodID constructor = env->GetMethodID(clazz, "<init>", "()V");
    jobject instance = env->NewObject(clazz, constructor);
    try {
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setFirst", "(Ljava/lang/String;)V"),
                            env->NewStringUTF(input.txid.c_str()));
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setSecond", "(I)V"), input.vout);
    } catch (const std::exception &e) {
        syslog(LOG_DEBUG, "[JNI] convert2JTxInput error::%s", e.what());
    }
    return instance;
}

jobject Deserializer::convert2JTxInputs(JNIEnv *env, const std::vector<TxInput> &inputs) {
    static auto arrayListClass = static_cast<jclass>(env->NewGlobalRef(
            env->FindClass("java/util/ArrayList")));
    static jmethodID constructor = env->GetMethodID(arrayListClass, "<init>", "()V");
    jmethodID addMethod = env->GetMethodID(arrayListClass, "add", "(Ljava/lang/Object;)Z");
    jobject arrayListInstance = env->NewObject(arrayListClass, constructor);
    for (const TxInput &input: inputs) {
        jobject element = convert2JTxInput(env, input);
        env->CallBooleanMethod(arrayListInstance, addMethod, element);
        env->DeleteLocalRef(element);
    }
    return arrayListInstance;
}

jobject Deserializer::convert2JTxOutput(JNIEnv *env, const TxOutput &output) {
    syslog(LOG_DEBUG, "[JNI] convert2JTxOutput()");
    jclass clazz = env->FindClass("com/nunchuk/android/model/TxOutput");
    jmethodID constructor = env->GetMethodID(clazz, "<init>", "()V");
    jobject instance = env->NewObject(clazz, constructor);
    try {
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setFirst", "(Ljava/lang/String;)V"),
                            env->NewStringUTF(output.first.c_str()));
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setSecond",
                                                       "(Lcom/nunchuk/android/model/Amount;)V"),
                            convert2JAmount(env, output.second));
    } catch (const std::exception &e) {
        syslog(LOG_DEBUG, "[JNI] convert2JTxOutput error::%s", e.what());
    }
    return instance;
}

jobject Deserializer::convert2JTxOutputs(JNIEnv *env, const std::vector<TxOutput> &outputs) {
    static auto arrayListClass = static_cast<jclass>(env->NewGlobalRef(
            env->FindClass("java/util/ArrayList")));
    static jmethodID constructor = env->GetMethodID(arrayListClass, "<init>", "()V");
    jmethodID addMethod = env->GetMethodID(arrayListClass, "add", "(Ljava/lang/Object;)Z");
    jobject arrayListInstance = env->NewObject(arrayListClass, constructor);
    for (const TxOutput &output: outputs) {
        jobject element = convert2JTxOutput(env, output);
        env->CallBooleanMethod(arrayListInstance, addMethod, element);
        env->DeleteLocalRef(element);
    }
    return arrayListInstance;
}

jobject Deserializer::convert2JAddressType(JNIEnv *env, const AddressType &type) {
    syslog(LOG_DEBUG, "[JNI] convert2JAddressType()");
    jstring className = env->NewStringUTF("com/nunchuk/android/type/AddressTypeHelper");
    jclass clazz = static_cast<jclass>(env->CallObjectMethod(Initializer::get()->classLoader,
                                                             Initializer::get()->loadClassMethod,
                                                             className));
    env->DeleteLocalRef(className);

    jmethodID staticMethod = env->GetStaticMethodID(clazz, "from",
                                                    "(I)Lcom/nunchuk/android/type/AddressType;");
    return env->CallStaticObjectMethod(clazz, staticMethod, (int) type);
}

jobject Deserializer::convert2JWalletType(JNIEnv *env, const WalletType &type) {
    syslog(LOG_DEBUG, "[JNI] convert2JWalletType()");
    jstring className = env->NewStringUTF("com/nunchuk/android/type/WalletTypeHelper");
    jclass clazz = static_cast<jclass>(env->CallObjectMethod(Initializer::get()->classLoader,
                                                             Initializer::get()->loadClassMethod,
                                                             className));
    env->DeleteLocalRef(className);

    jmethodID staticMethod = env->GetStaticMethodID(clazz, "from",
                                                    "(I)Lcom/nunchuk/android/type/WalletType;");
    return env->CallStaticObjectMethod(clazz, staticMethod, (int) type);
}

jobject Deserializer::convert2JSignerType(JNIEnv *env, const SignerType &type) {
    syslog(LOG_DEBUG, "[JNI] convert2JSignerType()");
    jstring className = env->NewStringUTF("com/nunchuk/android/type/SignerTypeHelper");
    jclass clazz = static_cast<jclass>(env->CallObjectMethod(Initializer::get()->classLoader,
                                                             Initializer::get()->loadClassMethod,
                                                             className));
    env->DeleteLocalRef(className);
    jmethodID staticMethod = env->GetStaticMethodID(clazz, "from",
                                                    "(I)Lcom/nunchuk/android/type/SignerType;");
    return env->CallStaticObjectMethod(clazz, staticMethod, ((int) type) + 1);
}

jobject Deserializer::convert2JTransactionStatus(JNIEnv *env, const TransactionStatus &status) {
    jclass clazz = env->FindClass("com/nunchuk/android/type/TransactionStatusHelper");
    jmethodID staticMethod = env->GetStaticMethodID(clazz, "from",
                                                    "(I)Lcom/nunchuk/android/type/TransactionStatus;");
    return env->CallStaticObjectMethod(clazz, staticMethod, (int) status);
}

jobject Deserializer::convert2JCoinStatus(JNIEnv *env, const CoinStatus &status) {
    jclass clazz = env->FindClass("com/nunchuk/android/type/CoinStatusHelper");
    jmethodID staticMethod = env->GetStaticMethodID(clazz, "from",
                                                    "(I)Lcom/nunchuk/android/type/CoinStatus;");
    return env->CallStaticObjectMethod(clazz, staticMethod, (int) status);
}

jobject Deserializer::convert2JBSMSData(JNIEnv *env, const BSMSData &data) {
    syslog(LOG_DEBUG, "[JNI] convert2JBSMSData()");
    jclass clazz = env->FindClass("com/nunchuk/android/model/BSMSData");
    jmethodID constructor = env->GetMethodID(clazz, "<init>", "()V");
    jobject instance = env->NewObject(clazz, constructor);
    try {
        env->CallVoidMethod(instance,
                            env->GetMethodID(clazz, "setVersion", "(Ljava/lang/String;)V"),
                            env->NewStringUTF(data.version.c_str()));
        env->CallVoidMethod(instance,
                            env->GetMethodID(clazz, "setDescriptor", "(Ljava/lang/String;)V"),
                            env->NewStringUTF(data.descriptor.c_str()));
        env->CallVoidMethod(instance,
                            env->GetMethodID(clazz, "setPathRestrictions", "(Ljava/lang/String;)V"),
                            env->NewStringUTF(data.path_restrictions.c_str()));
        env->CallVoidMethod(instance,
                            env->GetMethodID(clazz, "setFirstAddress", "(Ljava/lang/String;)V"),
                            env->NewStringUTF(data.first_address.c_str()));
    } catch (const std::exception &e) {
        syslog(LOG_DEBUG, "[JNI] convert2JSigner error::%s", e.what());
    }
    return instance;
}

jobject Deserializer::convert2JMasterSigner(JNIEnv *env, const MasterSigner &signer) {
    syslog(LOG_DEBUG, "[JNI] convert2JMasterSigner()");
    jclass clazz = env->FindClass("com/nunchuk/android/model/MasterSigner");
    jmethodID constructor = env->GetMethodID(clazz, "<init>", "()V");
    jobject instance = env->NewObject(clazz, constructor);
    try {
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setId", "(Ljava/lang/String;)V"),
                            env->NewStringUTF(signer.get_id().c_str()));
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setName", "(Ljava/lang/String;)V"),
                            env->NewStringUTF(signer.get_name().c_str()));
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setDevice",
                                                       "(Lcom/nunchuk/android/model/Device;)V"),
                            convert2JDevice(env, signer.get_device()));
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setLastHealthCheck", "(J)V"),
                            signer.get_last_health_check());
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setSoftware", "(Z)V"),
                            signer.is_software());
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setType",
                                                       "(Lcom/nunchuk/android/type/SignerType;)V"),
                            convert2JSignerType(env, signer.get_type()));
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setTags", "(Ljava/util/List;)V"),
                            convert2JSignerTags(env, signer.get_tags()));
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setVisible", "(Z)V"),
                            signer.is_visible());
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setNeedBackup", "(Z)V"),
                            signer.need_backup());
    } catch (const std::exception &e) {
        syslog(LOG_DEBUG, "[JNI] convert2JSigner error::%s", e.what());
    }
    return instance;
}

jobject Deserializer::convert2JSigner(JNIEnv *env, const SingleSigner &signer) {
    syslog(LOG_DEBUG, "[JNI] convert2JSigner()");
    jstring className = env->NewStringUTF("com/nunchuk/android/model/SingleSigner");
    jclass clazz = static_cast<jclass>(env->CallObjectMethod(Initializer::get()->classLoader,
                                                             Initializer::get()->loadClassMethod,
                                                             className));
    env->DeleteLocalRef(className);
    jmethodID constructor = env->GetMethodID(clazz, "<init>", "()V");
    jobject instance = env->NewObject(clazz, constructor);
    try {
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setName", "(Ljava/lang/String;)V"),
                            env->NewStringUTF(signer.get_name().c_str()));
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setXpub", "(Ljava/lang/String;)V"),
                            env->NewStringUTF(signer.get_xpub().c_str()));
        env->CallVoidMethod(instance,
                            env->GetMethodID(clazz, "setDerivationPath", "(Ljava/lang/String;)V"),
                            env->NewStringUTF(signer.get_derivation_path().c_str()));
        
        // Set external_internal_index
        auto external_internal_index = signer.get_external_internal_index();
        // Create Kotlin Pair object
        jstring pairClassName = env->NewStringUTF("kotlin/Pair");
        jclass pairClass = static_cast<jclass>(env->CallObjectMethod(Initializer::get()->classLoader,
                                                                     Initializer::get()->loadClassMethod,
                                                                     pairClassName));
        env->DeleteLocalRef(pairClassName);
        jmethodID pairConstructor = env->GetMethodID(pairClass, "<init>", "(Ljava/lang/Object;Ljava/lang/Object;)V");
        
        jclass integerClass = env->FindClass("java/lang/Integer");
        jmethodID integerConstructor = env->GetMethodID(integerClass, "<init>", "(I)V");
        jobject firstInt = env->NewObject(integerClass, integerConstructor, external_internal_index.first);
        jobject secondInt = env->NewObject(integerClass, integerConstructor, external_internal_index.second);
        
        // Create the Pair object
        jobject pairObject = env->NewObject(pairClass, pairConstructor, firstInt, secondInt);
        
        // Set the external_internal_index
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setExternalInternalIndex", "(Lkotlin/Pair;)V"),
                            pairObject);
        
        // Clean up local references
        env->DeleteLocalRef(pairClass);
        env->DeleteLocalRef(firstInt);
        env->DeleteLocalRef(secondInt);
        env->DeleteLocalRef(pairObject);
        
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setMasterFingerprint",
                                                       "(Ljava/lang/String;)V"),
                            env->NewStringUTF(signer.get_master_fingerprint().c_str()));
        env->CallVoidMethod(instance,
                            env->GetMethodID(clazz, "setPublicKey", "(Ljava/lang/String;)V"),
                            env->NewStringUTF(signer.get_public_key().c_str()));
        env->CallVoidMethod(instance,
                            env->GetMethodID(clazz, "setMasterSignerId", "(Ljava/lang/String;)V"),
                            env->NewStringUTF(signer.get_master_signer_id().c_str()));
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setLastHealthCheck", "(J)V"),
                            signer.get_last_health_check());
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setUsed", "(Z)V"), signer.is_used());
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setHasMasterSigner", "(Z)V"),
                            signer.has_master_signer());
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setType",
                                                       "(Lcom/nunchuk/android/type/SignerType;)V"),
                            convert2JSignerType(env, signer.get_type()));
        env->CallVoidMethod(instance,
                            env->GetMethodID(clazz, "setDescriptor", "(Ljava/lang/String;)V"),
                            env->NewStringUTF(signer.get_descriptor().c_str()));
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setTags", "(Ljava/util/List;)V"),
                            convert2JSignerTags(env, signer.get_tags()));
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setVisible", "(Z)V"),
                            signer.is_visible());
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setIndex", "(I)V"),
                            Utils::GetIndexFromPath(signer.get_derivation_path()));
    } catch (const std::exception &e) {
        syslog(LOG_DEBUG, "[JNI] convert2JSigner error::%s", e.what());
    }
    return instance;
}

jobject Deserializer::convert2JSigners(JNIEnv *env, const std::vector<SingleSigner> &signers) {
    syslog(LOG_DEBUG, "[JNI] convert2JSigners()");
    static auto arrayListClass = static_cast<jclass>(env->NewGlobalRef(
            env->FindClass("java/util/ArrayList")));
    static jmethodID constructor = env->GetMethodID(arrayListClass, "<init>", "()V");
    jmethodID addMethod = env->GetMethodID(arrayListClass, "add", "(Ljava/lang/Object;)Z");
    jobject arrayListInstance = env->NewObject(arrayListClass, constructor);
    for (const SingleSigner &s: signers) {
        jobject element = convert2JSigner(env, s);
        env->CallBooleanMethod(arrayListInstance, addMethod, element);
        env->DeleteLocalRef(element);
    }
    return arrayListInstance;
}

jobject
Deserializer::convert2JMasterSigners(JNIEnv *env, const std::vector<MasterSigner> &signers) {
    syslog(LOG_DEBUG, "[JNI] convert2JMasterSigners()");
    static auto arrayListClass = static_cast<jclass>(env->NewGlobalRef(
            env->FindClass("java/util/ArrayList")));
    static jmethodID constructor = env->GetMethodID(arrayListClass, "<init>", "()V");
    jmethodID addMethod = env->GetMethodID(arrayListClass, "add", "(Ljava/lang/Object;)Z");
    jobject arrayListInstance = env->NewObject(arrayListClass, constructor);
    for (const MasterSigner &s: signers) {
        jobject element = convert2JMasterSigner(env, s);
        env->CallBooleanMethod(arrayListInstance, addMethod, element);
        env->DeleteLocalRef(element);
    }
    return arrayListInstance;
}

jobject Deserializer::convert2JWallet(JNIEnv *env, const Wallet &wallet) {
    jclass clazz = env->FindClass("com/nunchuk/android/model/Wallet");
    jmethodID constructor = env->GetMethodID(clazz, "<init>", "()V");
    jobject instance = env->NewObject(clazz, constructor);
    try {
        jobject signers = convert2JSigners(env, wallet.get_signers());
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setId", "(Ljava/lang/String;)V"),
                            env->NewStringUTF(wallet.get_id().c_str()));
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setName", "(Ljava/lang/String;)V"),
                            env->NewStringUTF(wallet.get_name().c_str()));
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setTotalRequireSigns", "(I)V"),
                            wallet.get_m());
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setSigners", "(Ljava/util/List;)V"),
                            signers);
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setEscrow", "(Z)V"),
                            wallet.is_escrow());
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setBalance",
                                                       "(Lcom/nunchuk/android/model/Amount;)V"),
                            convert2JAmount(env, wallet.get_unconfirmed_balance()));
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setAddressType",
                                                       "(Lcom/nunchuk/android/type/AddressType;)V"),
                            convert2JAddressType(env, wallet.get_address_type()));
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setCreateDate", "(J)V"),
                            wallet.get_create_date());
        env->CallVoidMethod(instance,
                            env->GetMethodID(clazz, "setDescription", "(Ljava/lang/String;)V"),
                            env->NewStringUTF(wallet.get_description().c_str()));
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setGapLimit", "(I)V"),
                            wallet.get_gap_limit());
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setNeedBackup", "(Z)V"),
                            wallet.need_backup());
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setWalletTemplate",
                                                       "(Lcom/nunchuk/android/type/WalletTemplate;)V"),
                            convert2JWalletTemplate(env, wallet.get_wallet_template()));
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setArchived", "(Z)V"), wallet.is_archived());
        env->CallVoidMethod(instance,
                            env->GetMethodID(clazz, "setMiniscript", "(Ljava/lang/String;)V"),
                            env->NewStringUTF(wallet.get_miniscript().c_str()));
        syslog(LOG_DEBUG, "[JNI] convert2JWallet balance::%s",
               Utils::ValueFromAmount(wallet.get_balance()).c_str());
    } catch (std::exception &e) {
        syslog(LOG_DEBUG, "[JNI] convert2JWallet error::%s", e.what());
    }
    return instance;
}

jobject Deserializer::convert2JWallets(JNIEnv *env, const std::vector<Wallet> &wallets) {
    syslog(LOG_DEBUG, "[JNI] convert2JWallets()");
    static auto arrayListClass = static_cast<jclass>(env->NewGlobalRef(
            env->FindClass("java/util/ArrayList")));
    static jmethodID constructor = env->GetMethodID(arrayListClass, "<init>", "()V");
    jobject arrayListInstance = env->NewObject(arrayListClass, constructor);
    jmethodID addMethod = env->GetMethodID(arrayListClass, "add", "(Ljava/lang/Object;)Z");
    if (wallets.empty()) {
        syslog(LOG_DEBUG, "[JNI] wallets empty");
        return arrayListInstance;
    } else {
        syslog(LOG_DEBUG, "[JNI] wallets size:: %lu", wallets.size());
        for (const Wallet &s: wallets) {
            jobject element = convert2JWallet(env, s);
            env->CallBooleanMethod(arrayListInstance, addMethod, element);
            env->DeleteLocalRef(element);
        }
    }
    return arrayListInstance;
}

jobject Deserializer::convert2JTransaction(JNIEnv *env, const Transaction &transaction,
                                           const Amount amount) {
    syslog(LOG_DEBUG, "[JNI] convert2JTransaction()");
    jclass clazz = env->FindClass("com/nunchuk/android/model/Transaction");
    jmethodID constructor = env->GetMethodID(clazz, "<init>", "()V");
    jobject instance = env->NewObject(clazz, constructor);
    try {
        Amount total = (transaction.is_receive() ? transaction.get_sub_amount() : (
                transaction.get_sub_amount() + transaction.get_fee()));
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setTxId", "(Ljava/lang/String;)V"),
                            env->NewStringUTF(transaction.get_txid().c_str()));
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setHeight", "(I)V"),
                            transaction.get_height());
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setInputs", "(Ljava/util/List;)V"),
                            convert2JTxInputs(env, transaction.get_inputs()));
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setOutputs", "(Ljava/util/List;)V"),
                            convert2JTxOutputs(env, transaction.get_outputs()));
        env->CallVoidMethod(instance,
                            env->GetMethodID(clazz, "setReceiveOutputs", "(Ljava/util/List;)V"),
                            convert2JTxOutputs(env, transaction.get_receive_outputs()));
        env->CallVoidMethod(instance,
                            env->GetMethodID(clazz, "setUserOutputs", "(Ljava/util/List;)V"),
                            convert2JTxOutputs(env, transaction.get_user_outputs()));
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setChangeIndex", "(I)V"),
                            transaction.get_change_index());
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setM", "(I)V"), transaction.get_m());
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setSigners", "(Ljava/util/Map;)V"),
                            convert2JStringBooleanMap(env, transaction.get_signers()));
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setMemo", "(Ljava/lang/String;)V"),
                            env->NewStringUTF(transaction.get_memo().c_str()));
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setStatus",
                                                       "(Lcom/nunchuk/android/type/TransactionStatus;)V"),
                            convert2JTransactionStatus(env, transaction.get_status()));
        env->CallVoidMethod(instance,
                            env->GetMethodID(clazz, "setReplacedByTxid", "(Ljava/lang/String;)V"),
                            env->NewStringUTF(transaction.get_replaced_by_txid().c_str()));
        env->CallVoidMethod(instance,
                            env->GetMethodID(clazz, "setReplacedTxid", "(Ljava/lang/String;)V"),
                            env->NewStringUTF(transaction.get_replace_txid().c_str()));
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setFee",
                                                       "(Lcom/nunchuk/android/model/Amount;)V"),
                            convert2JAmount(env, transaction.get_fee()));
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setFeeRate",
                                                       "(Lcom/nunchuk/android/model/Amount;)V"),
                            convert2JAmount(env, transaction.get_fee_rate()));
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setBlockTime", "(J)V"),
                            transaction.get_blocktime());
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setSubtractFeeFromAmount", "(Z)V"),
                            transaction.subtract_fee_from_amount());
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setReceive", "(Z)V"),
                            transaction.is_receive());
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setSubAmount",
                                                       "(Lcom/nunchuk/android/model/Amount;)V"),
                            convert2JAmount(env, transaction.get_sub_amount()));
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setTotalAmount",
                                                       "(Lcom/nunchuk/android/model/Amount;)V"),
                            convert2JAmount(env, total));
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setPsbt", "(Ljava/lang/String;)V"),
                            env->NewStringUTF(transaction.get_psbt().c_str()));
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setCpfpFee",
                                                       "(Lcom/nunchuk/android/model/Amount;)V"),
                            convert2JAmount(env, amount));
        env->CallVoidMethod(instance,
                            env->GetMethodID(clazz, "setKeySetStatus", "(Ljava/util/List;)V"),
                            convert2JKeySetStatus(env, transaction.get_keyset_status()));
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setLockTime", "(J)V"),
                            static_cast<jlong>(transaction.get_lock_time()));
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setRaw", "(Ljava/lang/String;)V"),
                            env->NewStringUTF(transaction.get_raw().c_str()));
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setSignedSigner", "(Ljava/util/List;)V"),
                            convert2JSigners(env, transaction.get_signed()));
    } catch (std::exception &e) {
        syslog(LOG_DEBUG, "[JNI] convert2JTransaction error::%s", e.what());
    }
    return instance;
}

jobject
Deserializer::convert2JKeySetStatus(JNIEnv *env, const std::vector<KeysetStatus> &keySetStatus) {
    static auto arrayListClass = static_cast<jclass>(env->NewGlobalRef(
            env->FindClass("java/util/ArrayList")));
    static jmethodID arrayListConstructor = env->GetMethodID(arrayListClass, "<init>", "()V");
    jmethodID arrayListAddMethod = env->GetMethodID(arrayListClass, "add", "(Ljava/lang/Object;)Z");
    jobject arrayListInstance = env->NewObject(arrayListClass, arrayListConstructor);

    for (const auto &status: keySetStatus) {
        jclass keySetStatusClass = env->FindClass("com/nunchuk/android/model/KeySetStatus");
        jmethodID keySetStatusConstructor = env->GetMethodID(keySetStatusClass, "<init>", "()V");
        jobject keySetStatusInstance = env->NewObject(keySetStatusClass, keySetStatusConstructor);

        env->CallVoidMethod(keySetStatusInstance, env->GetMethodID(keySetStatusClass, "setStatus",
                                                                   "(Lcom/nunchuk/android/type/TransactionStatus;)V"),
                            convert2JTransactionStatus(env, status.first));
        env->CallVoidMethod(keySetStatusInstance,
                            env->GetMethodID(keySetStatusClass, "setSignerStatus",
                                             "(Ljava/util/Map;)V"),
                            convert2JStringBooleanMap(env, status.second));

        env->CallBooleanMethod(arrayListInstance, arrayListAddMethod, keySetStatusInstance);
        env->DeleteLocalRef(keySetStatusInstance);
    }

    return arrayListInstance;
}

jobject Deserializer::convert2JTransaction(JNIEnv *env, const Transaction &transaction) {
    return convert2JTransaction(env, transaction, 0);
}

jobject
Deserializer::convert2JTransactions(JNIEnv *env, const std::vector<Transaction> &transactions) {
    syslog(LOG_DEBUG, "[JNI] convert2JMasterSigners()");
    static auto arrayListClass = static_cast<jclass>(env->NewGlobalRef(
            env->FindClass("java/util/ArrayList")));
    static jmethodID constructor = env->GetMethodID(arrayListClass, "<init>", "()V");
    jmethodID addMethod = env->GetMethodID(arrayListClass, "add", "(Ljava/lang/Object;)Z");
    jobject arrayListInstance = env->NewObject(arrayListClass, constructor);
    for (const Transaction &s: transactions) {
        jobject element = convert2JTransaction(env, s);
        env->CallBooleanMethod(arrayListInstance, addMethod, element);
        env->DeleteLocalRef(element);
    }
    return arrayListInstance;
}

jobject Deserializer::convert2JMatrixEvent(JNIEnv *env, const NunchukMatrixEvent &event) {
    syslog(LOG_DEBUG, "[JNI] convert2JMatrixEvent()");
    jclass clazz = env->FindClass("com/nunchuk/android/model/NunchukMatrixEvent");
    jmethodID constructor = env->GetMethodID(clazz, "<init>", "()V");
    jobject instance = env->NewObject(clazz, constructor);
    try {
        syslog(LOG_DEBUG, "[JNI]content::%s", event.get_content().c_str());
        syslog(LOG_DEBUG, "[JNI]roomId::%s", event.get_room_id().c_str());
        syslog(LOG_DEBUG, "[JNI]sender::%s", event.get_sender().c_str());
        syslog(LOG_DEBUG, "[JNI]time::%ld", event.get_ts());
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setType", "(Ljava/lang/String;)V"),
                            env->NewStringUTF(event.get_type().c_str()));
        env->CallVoidMethod(instance,
                            env->GetMethodID(clazz, "setContent", "(Ljava/lang/String;)V"),
                            env->NewStringUTF(event.get_content().c_str()));
        env->CallVoidMethod(instance,
                            env->GetMethodID(clazz, "setEventId", "(Ljava/lang/String;)V"),
                            env->NewStringUTF(event.get_event_id().c_str()));
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setRoomId", "(Ljava/lang/String;)V"),
                            env->NewStringUTF(event.get_room_id().c_str()));
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setSender", "(Ljava/lang/String;)V"),
                            env->NewStringUTF(event.get_sender().c_str()));
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setTime", "(J)V"), event.get_ts());
    } catch (const std::exception &e) {
        syslog(LOG_DEBUG, "[JNI] convert2JMatrixEvent error::%s", e.what());
    }
    return instance;
}

jobject Deserializer::convert2JRoomWallet(JNIEnv *env, const RoomWallet &wallet) {
    syslog(LOG_DEBUG, "[JNI] convert2JRoomWallet()");
    jclass clazz = env->FindClass("com/nunchuk/android/model/RoomWallet");
    jmethodID constructor = env->GetMethodID(clazz, "<init>", "()V");
    jobject instance = env->NewObject(clazz, constructor);
    try {
        syslog(LOG_DEBUG, "[JNI]walletId::%s", wallet.get_wallet_id().c_str());
        syslog(LOG_DEBUG, "[JNI]roomId::%s", wallet.get_room_id().c_str());
        syslog(LOG_DEBUG, "[JNI]jsonContent::%s", wallet.get_json_content().c_str());
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setRoomId", "(Ljava/lang/String;)V"),
                            env->NewStringUTF(wallet.get_room_id().c_str()));
        env->CallVoidMethod(instance,
                            env->GetMethodID(clazz, "setWalletId", "(Ljava/lang/String;)V"),
                            env->NewStringUTF(wallet.get_wallet_id().c_str()));
        env->CallVoidMethod(instance,
                            env->GetMethodID(clazz, "setInitEventId", "(Ljava/lang/String;)V"),
                            env->NewStringUTF(wallet.get_init_event_id().c_str()));
        env->CallVoidMethod(instance,
                            env->GetMethodID(clazz, "setJoinEventIds", "(Ljava/util/List;)V"),
                            convert2JListString(env, wallet.get_join_event_ids()));
        env->CallVoidMethod(instance,
                            env->GetMethodID(clazz, "setLeaveEventIds", "(Ljava/util/List;)V"),
                            convert2JListString(env, wallet.get_leave_event_ids()));
        env->CallVoidMethod(instance,
                            env->GetMethodID(clazz, "setFinalizeEventId", "(Ljava/lang/String;)V"),
                            env->NewStringUTF(wallet.get_finalize_event_id().c_str()));
        env->CallVoidMethod(instance,
                            env->GetMethodID(clazz, "setCancelEventId", "(Ljava/lang/String;)V"),
                            env->NewStringUTF(wallet.get_cancel_event_id().c_str()));
        env->CallVoidMethod(instance,
                            env->GetMethodID(clazz, "setReadyEventId", "(Ljava/lang/String;)V"),
                            env->NewStringUTF(wallet.get_ready_event_id().c_str()));
        env->CallVoidMethod(instance,
                            env->GetMethodID(clazz, "setDeleteEventId", "(Ljava/lang/String;)V"),
                            env->NewStringUTF(wallet.get_delete_event_id().c_str()));
        env->CallVoidMethod(instance,
                            env->GetMethodID(clazz, "setJsonContent", "(Ljava/lang/String;)V"),
                            env->NewStringUTF(wallet.get_json_content().c_str()));
    } catch (const std::exception &e) {
        syslog(LOG_DEBUG, "[JNI] convert2JRoomWallet error::%s", e.what());
    }
    return instance;
}

jobject Deserializer::convert2JRoomWallets(JNIEnv *env, const std::vector<RoomWallet> &wallets) {
    syslog(LOG_DEBUG, "[JNI] convert2JRoomWallets()");
    static auto arrayListClass = static_cast<jclass>(env->NewGlobalRef(
            env->FindClass("java/util/ArrayList")));
    static jmethodID constructor = env->GetMethodID(arrayListClass, "<init>", "()V");
    jmethodID addMethod = env->GetMethodID(arrayListClass, "add", "(Ljava/lang/Object;)Z");
    jobject arrayListInstance = env->NewObject(arrayListClass, constructor);
    for (const RoomWallet &s: wallets) {
        jobject element = convert2JRoomWallet(env, s);
        env->CallBooleanMethod(arrayListInstance, addMethod, element);
        env->DeleteLocalRef(element);
    }
    return arrayListInstance;
}

jobject Deserializer::convert2JRoomTransaction(JNIEnv *env, const RoomTransaction &transaction) {
    syslog(LOG_DEBUG, "[JNI] convert2JRoomTransaction()");
    jclass clazz = env->FindClass("com/nunchuk/android/model/RoomTransaction");
    jmethodID constructor = env->GetMethodID(clazz, "<init>", "()V");
    jobject instance = env->NewObject(clazz, constructor);
    try {
        syslog(LOG_DEBUG, "[JNI]walletId::%s", transaction.get_wallet_id().c_str());
        syslog(LOG_DEBUG, "[JNI]roomId::%s", transaction.get_room_id().c_str());
        syslog(LOG_DEBUG, "[JNI]txId::%s", transaction.get_tx_id().c_str());
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setRoomId", "(Ljava/lang/String;)V"),
                            env->NewStringUTF(transaction.get_room_id().c_str()));
        env->CallVoidMethod(instance,
                            env->GetMethodID(clazz, "setWalletId", "(Ljava/lang/String;)V"),
                            env->NewStringUTF(transaction.get_wallet_id().c_str()));
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setTxId", "(Ljava/lang/String;)V"),
                            env->NewStringUTF(transaction.get_tx_id().c_str()));
        env->CallVoidMethod(instance,
                            env->GetMethodID(clazz, "setInitEventId", "(Ljava/lang/String;)V"),
                            env->NewStringUTF(transaction.get_init_event_id().c_str()));
        env->CallVoidMethod(instance,
                            env->GetMethodID(clazz, "setSignEventIds", "(Ljava/util/List;)V"),
                            convert2JListString(env, transaction.get_sign_event_ids()));
        env->CallVoidMethod(instance,
                            env->GetMethodID(clazz, "setRejectEventIds", "(Ljava/util/List;)V"),
                            convert2JListString(env, transaction.get_reject_event_ids()));
        env->CallVoidMethod(instance,
                            env->GetMethodID(clazz, "setBroadcastEventId", "(Ljava/lang/String;)V"),
                            env->NewStringUTF(transaction.get_broadcast_event_id().c_str()));
        env->CallVoidMethod(instance,
                            env->GetMethodID(clazz, "setCancelEventId", "(Ljava/lang/String;)V"),
                            env->NewStringUTF(transaction.get_cancel_event_id().c_str()));
        env->CallVoidMethod(instance,
                            env->GetMethodID(clazz, "setReadyEventId", "(Ljava/lang/String;)V"),
                            env->NewStringUTF(transaction.get_ready_event_id().c_str()));
    } catch (const std::exception &e) {
        syslog(LOG_DEBUG, "[JNI] convert2JRoomTransaction error::%s", e.what());
    }
    return instance;
}

jobject Deserializer::convert2JRoomTransactions(JNIEnv *env,
                                                const std::vector<RoomTransaction> &transactions) {
    syslog(LOG_DEBUG, "[JNI] convert2JRoomTransactions()");
    static auto arrayListClass = static_cast<jclass>(env->NewGlobalRef(
            env->FindClass("java/util/ArrayList")));
    static jmethodID constructor = env->GetMethodID(arrayListClass, "<init>", "()V");
    jmethodID addMethod = env->GetMethodID(arrayListClass, "add", "(Ljava/lang/Object;)Z");
    jobject arrayListInstance = env->NewObject(arrayListClass, constructor);
    for (const RoomTransaction &s: transactions) {
        jobject element = convert2JRoomTransaction(env, s);
        env->CallBooleanMethod(arrayListInstance, addMethod, element);
        env->DeleteLocalRef(element);
    }
    return arrayListInstance;
}

jobject Deserializer::convert2JConnectionStatus(JNIEnv *env, const ConnectionStatus &status) {
    jclass clazz = env->FindClass("com/nunchuk/android/type/ConnectionStatusTypeHelper");
    jmethodID staticMethod = env->GetStaticMethodID(clazz, "from",
                                                    "(I)Lcom/nunchuk/android/type/ConnectionStatus;");
    return env->CallStaticObjectMethod(clazz, staticMethod, (int) status);
}

jobject Deserializer::convert2JHealthStatus(JNIEnv *env, const HealthStatus &status) {
    jclass clazz = env->FindClass("com/nunchuk/android/type/HealthStatusTypeHelper");
    jmethodID staticMethod = env->GetStaticMethodID(clazz, "from",
                                                    "(I)Lcom/nunchuk/android/type/HealthStatus;");
    return env->CallStaticObjectMethod(clazz, staticMethod, (int) status);
}

jobject Deserializer::convert2JTapSignerStatus(JNIEnv *env, const TapsignerStatus &status) {
    syslog(LOG_DEBUG, "[JNI] convert2JTapSignerStatus()");
    jclass clazz = env->FindClass("com/nunchuk/android/model/TapSignerStatus");
    jmethodID constructor = env->GetMethodID(clazz, "<init>", "()V");
    jobject instance = env->NewObject(clazz, constructor);
    try {
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setNeedSetup", "(Z)V"),
                            status.need_setup());
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setCreateSigner", "(Z)V"),
                            status.is_master_signer());
        auto back_up_data = status.get_backup_data();
        jbyteArray ret = env->NewByteArray(back_up_data.size());
        env->SetByteArrayRegion(ret, 0, back_up_data.size(), (jbyte *) back_up_data.data());
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setBackupKey", "([B)V"), ret);
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setAuthDelayInSecond", "(I)V"),
                            status.get_auth_delay());
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setBirthHeight", "(I)V"),
                            status.get_birth_height());
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setIdent", "(Ljava/lang/String;)V"),
                            env->NewStringUTF(status.get_card_ident().c_str()));
        env->CallVoidMethod(instance,
                            env->GetMethodID(clazz, "setMasterSignerId", "(Ljava/lang/String;)V"),
                            env->NewStringUTF(status.get_master_signer_id().c_str()));
        env->CallVoidMethod(instance,
                            env->GetMethodID(clazz, "setVersion", "(Ljava/lang/String;)V"),
                            env->NewStringUTF(status.get_version().c_str()));
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setTestNet", "(Z)V"),
                            status.is_testnet());
    } catch (const std::exception &e) {
        syslog(LOG_DEBUG, "[JNI] convert2JTapSignerStatus error::%s", e.what());
    }
    return instance;
}

jobject Deserializer::convert2JSatsCardSlotStatus(JNIEnv *env, const SatscardSlot::Status &status) {
    syslog(LOG_DEBUG, "[JNI] convert2JSatsCardSlotStatus()");
    jclass clazz = env->FindClass("com/nunchuk/android/type/SatsCardSlotStatusHelper");
    jmethodID staticMethod = env->GetStaticMethodID(clazz, "from",
                                                    "(I)Lcom/nunchuk/android/type/SatsCardSlotStatus;");
    return env->CallStaticObjectMethod(clazz, staticMethod, (int) status);
}

jobject Deserializer::convert2JSatsCardSlot(JNIEnv *env, const SatscardSlot &slot) {
    syslog(LOG_DEBUG, "[JNI] convert2JSatsCardSlot()");
    jclass clazz = env->FindClass("com/nunchuk/android/model/SatsCardSlot");
    jmethodID constructor = env->GetMethodID(clazz, "<init>", "()V");
    jobject instance = env->NewObject(clazz, constructor);
    try {
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setIndex", "(I)V"),
                            slot.get_index());
        env->CallVoidMethod(instance,
                            env->GetMethodID(clazz, "setAddress", "(Ljava/lang/String;)V"),
                            env->NewStringUTF(slot.get_address().c_str()));
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setStatus",
                                                       "(Lcom/nunchuk/android/type/SatsCardSlotStatus;)V"),
                            convert2JSatsCardSlotStatus(env, slot.get_status()));
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setConfirmed", "(Z)V"),
                            slot.is_confirmed());
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setBalance",
                                                       "(Lcom/nunchuk/android/model/Amount;)V"),
                            convert2JAmount(env, slot.get_balance()));
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setOutputs", "(Ljava/util/List;)V"),
                            convert2JUnspentOutputs(env, slot.get_utxos()));
        auto priv_key = slot.get_privkey();
        jbyteArray private_key_byte_array = env->NewByteArray(priv_key.size());
        env->SetByteArrayRegion(private_key_byte_array, 0, priv_key.size(),
                                (jbyte *) priv_key.data());
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setPrivateKey", "([B)V"),
                            private_key_byte_array);

        auto public_key = slot.get_pubkey();
        jbyteArray public_key_byte_array = env->NewByteArray(public_key.size());
        env->SetByteArrayRegion(public_key_byte_array, 0, public_key.size(),
                                (jbyte *) public_key.data());
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setPublicKey", "([B)V"),
                            public_key_byte_array);

        auto chain_code = slot.get_chain_code();
        jbyteArray chain_code_array = env->NewByteArray(chain_code.size());
        env->SetByteArrayRegion(chain_code_array, 0, chain_code.size(),
                                (jbyte *) chain_code.data());
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setChainCode", "([B)V"),
                            chain_code_array);

        auto master_private_key = slot.get_master_privkey();
        jbyteArray master_private_key_array = env->NewByteArray(master_private_key.size());
        env->SetByteArrayRegion(master_private_key_array, 0, master_private_key.size(),
                                (jbyte *) master_private_key.data());
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setMasterPrivateKey", "([B)V"),
                            master_private_key_array);
    } catch (const std::exception &e) {
        syslog(LOG_DEBUG, "[JNI] convert2JSatsCardSlotStatus error::%s", e.what());
    }
    return instance;
}

jobject Deserializer::convert2JSatsCardStatus(JNIEnv *env, const SatscardStatus &status) {
    syslog(LOG_DEBUG, "[JNI] convert2JSatsCardStatus()");
    jclass clazz = env->FindClass("com/nunchuk/android/model/SatsCardStatus");
    jmethodID constructor = env->GetMethodID(clazz, "<init>", "()V");
    jobject instance = env->NewObject(clazz, constructor);
    try {
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setNeedSetup", "(Z)V"),
                            status.need_setup());
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setActiveSlotIndex", "(I)V"),
                            status.get_active_slot_index());
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setNumberOfSlot", "(I)V"),
                            status.get_number_of_slots());
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setUsedUp", "(Z)V"),
                            status.is_used_up());
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setIdent", "(Ljava/lang/String;)V"),
                            env->NewStringUTF(status.get_card_ident().c_str()));
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setAuthDelayInSecond", "(I)V"),
                            status.get_auth_delay());
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setSlots", "(Ljava/util/List;)V"),
                            convert2JSatsCardSlots(env, status.get_slots()));
    } catch (const std::exception &e) {
        syslog(LOG_DEBUG, "[JNI] convert2JSatsCardStatus error::%s", e.what());
    }
    return instance;
}

jobject Deserializer::convert2JSatsCardSlots(JNIEnv *env, const std::vector<SatscardSlot> &slots) {
    static auto arrayListClass = static_cast<jclass>(env->NewGlobalRef(
            env->FindClass("java/util/ArrayList")));
    static jmethodID constructor = env->GetMethodID(arrayListClass, "<init>", "()V");
    jmethodID addMethod = env->GetMethodID(arrayListClass, "add", "(Ljava/lang/Object;)Z");
    jobject arrayListInstance = env->NewObject(arrayListClass, constructor);
    for (const SatscardSlot &slot: slots) {
        jobject element = convert2JSatsCardSlot(env, slot);
        env->CallBooleanMethod(arrayListInstance, addMethod, element);
        env->DeleteLocalRef(element);
    }
    return arrayListInstance;
}

jobject Deserializer::convert2JUnspentOutput(JNIEnv *env, const UnspentOutput &output) {
    syslog(LOG_DEBUG, "[JNI] convert2JUnspentOutput()");
    jclass clazz = env->FindClass("com/nunchuk/android/model/UnspentOutput");
    jmethodID constructor = env->GetMethodID(clazz, "<init>", "()V");
    jobject instance = env->NewObject(clazz, constructor);
    try {
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setTxid", "(Ljava/lang/String;)V"),
                            env->NewStringUTF(output.get_txid().c_str()));
        env->CallVoidMethod(instance,
                            env->GetMethodID(clazz, "setAddress", "(Ljava/lang/String;)V"),
                            env->NewStringUTF(output.get_address().c_str()));
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setVout", "(I)V"),
                            output.get_vout());
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setHeight", "(I)V"),
                            output.get_height());
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setMemo", "(Ljava/lang/String;)V"),
                            env->NewStringUTF(output.get_memo().c_str()));
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setChange", "(Z)V"),
                            output.is_change());
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setLocked", "(Z)V"),
                            output.is_locked());
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setTime", "(J)V"),
                            output.get_blocktime());
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setScheduleTime", "(J)V"),
                            output.get_schedule_time());
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setStatus",
                                                       "(Lcom/nunchuk/android/type/CoinStatus;)V"),
                            convert2JCoinStatus(env, output.get_status()));
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setTags", "(Ljava/util/Set;)V"),
                            convert2JInts(env, output.get_tags()));
        env->CallVoidMethod(instance,

                            env->GetMethodID(clazz, "setCollection", "(Ljava/util/Set;)V"),
                            convert2JInts(env, output.get_collections()));
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setAmount",
                                                       "(Lcom/nunchuk/android/model/Amount;)V"),
                            convert2JAmount(env, output.get_amount()));
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setLockBased",
                                                       "(Lcom/nunchuk/android/type/MiniscriptTimelockBased;)V"),
                            convert2JMiniscriptTimelockBased(env, output.get_lock_based()));
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setTimelocks", "(Ljava/util/List;)V"),
                            convert2JLongs(env, output.get_timelocks()));
    } catch (const std::exception &e) {
        syslog(LOG_DEBUG, "[JNI] convert2JUnspentOutput error::%s", e.what());
    }
    return instance;
}

jobject
Deserializer::convert2JUnspentOutputs(JNIEnv *env, const std::vector<UnspentOutput> &outputs) {
    static auto arrayListClass = static_cast<jclass>(env->NewGlobalRef(
            env->FindClass("java/util/ArrayList")));
    static jmethodID constructor = env->GetMethodID(arrayListClass, "<init>", "()V");
    jmethodID addMethod = env->GetMethodID(arrayListClass, "add", "(Ljava/lang/Object;)Z");
    jobject arrayListInstance = env->NewObject(arrayListClass, constructor);
    for (const UnspentOutput &output: outputs) {
        jobject element = convert2JUnspentOutput(env, output);
        env->CallBooleanMethod(arrayListInstance, addMethod, element);
        env->DeleteLocalRef(element);
    }
    return arrayListInstance;
}

jbyteArray Deserializer::convert2JByteArray(JNIEnv *env, const std::vector<unsigned char> &bytes) {
    jbyteArray ret = env->NewByteArray(bytes.size());
    env->SetByteArrayRegion(ret, 0, bytes.size(), reinterpret_cast<const jbyte *>(bytes.data()));
    return ret;
}

jobject Deserializer::convert2JRecord(JNIEnv *env, const NDEFRecord &record) {
    static auto NdefRecordClass = (jclass) env->NewGlobalRef(
            env->FindClass("android/nfc/NdefRecord"));
    jmethodID constructor = env->GetMethodID(NdefRecordClass, "<init>", "(S[B[B[B)V");
    jobject ret = env->NewObject(NdefRecordClass, constructor,
                                 record.typeNameFormat,
                                 convert2JByteArray(env, record.type),
                                 convert2JByteArray(env, record.id),
                                 convert2JByteArray(env, record.payload)
    );
    return ret;
}

jobjectArray Deserializer::convert2JRecords(JNIEnv *env, const std::vector<NDEFRecord> &records) {
    static auto NdefRecordClass = (jclass) env->NewGlobalRef(
            env->FindClass("android/nfc/NdefRecord"));
    jobjectArray ret = env->NewObjectArray(records.size(), NdefRecordClass, nullptr);
    for (int i = 0; i < records.size(); ++i) {
        env->SetObjectArrayElement(ret, i, convert2JRecord(env, records[i]));
    }
    return ret;
}

jobject Deserializer::convert2JPrimaryKeys(JNIEnv *env, const std::vector<PrimaryKey> &keys) {
    syslog(LOG_DEBUG, "[JNI] convert2JPrimaryKeys()");
    static auto arrayListClass = static_cast<jclass>(env->NewGlobalRef(
            env->FindClass("java/util/ArrayList")));
    static jmethodID constructor = env->GetMethodID(arrayListClass, "<init>", "()V");
    jmethodID addMethod = env->GetMethodID(arrayListClass, "add", "(Ljava/lang/Object;)Z");
    jobject arrayListInstance = env->NewObject(arrayListClass, constructor);
    for (const PrimaryKey &s: keys) {
        jobject element = convert2JPrimaryKey(env, s);
        env->CallBooleanMethod(arrayListInstance, addMethod, element);
        env->DeleteLocalRef(element);
    }
    return arrayListInstance;
}

jobject Deserializer::convert2JPrimaryKey(JNIEnv *env, const PrimaryKey &key) {
    syslog(LOG_DEBUG, "[JNI] convert2JPrimaryKey()");
    jclass clazz = env->FindClass("com/nunchuk/android/model/PrimaryKey");
    jmethodID constructor = env->GetMethodID(clazz, "<init>", "()V");
    jobject instance = env->NewObject(clazz, constructor);
    try {
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setName", "(Ljava/lang/String;)V"),
                            env->NewStringUTF(key.get_name().c_str()));
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setMasterFingerprint",
                                                       "(Ljava/lang/String;)V"),
                            env->NewStringUTF(key.get_master_fingerprint().c_str()));
        env->CallVoidMethod(instance,
                            env->GetMethodID(clazz, "setAccount", "(Ljava/lang/String;)V"),
                            env->NewStringUTF(key.get_account().c_str()));
        env->CallVoidMethod(instance,
                            env->GetMethodID(clazz, "setAddress", "(Ljava/lang/String;)V"),
                            env->NewStringUTF(key.get_address().c_str()));
        env->CallVoidMethod(instance,
                            env->GetMethodID(clazz, "setDecoyPin", "(Ljava/lang/String;)V"),
                            env->NewStringUTF(key.get_decoy_pin().c_str()));
    } catch (const std::exception &e) {
        syslog(LOG_DEBUG, "[JNI] convert2JSigner error::%s", e.what());
    }
    return instance;
}

jobject Deserializer::convert2JBtcUri(JNIEnv *env, const BtcUri &btcUri) {
    syslog(LOG_DEBUG, "[JNI] convert2JBtcUri()");
    jclass clazz = env->FindClass("com/nunchuk/android/model/BtcUri");
    jmethodID constructor = env->GetMethodID(clazz, "<init>", "()V");
    jobject instance = env->NewObject(clazz, constructor);
    try {
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setAmount",
                                                       "(Lcom/nunchuk/android/model/Amount;)V"),
                            convert2JAmount(env, btcUri.amount));
        env->CallVoidMethod(instance,
                            env->GetMethodID(clazz, "setAddress", "(Ljava/lang/String;)V"),
                            env->NewStringUTF(btcUri.address.c_str()));
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setLabel", "(Ljava/lang/String;)V"),
                            env->NewStringUTF(btcUri.label.c_str()));
        env->CallVoidMethod(instance,
                            env->GetMethodID(clazz, "setMessage", "(Ljava/lang/String;)V"),
                            env->NewStringUTF(btcUri.message.c_str()));
    } catch (const std::exception &e) {
        syslog(LOG_DEBUG, "[JNI] convert2JBtcUri error::%s", e.what());
    }
    return instance;
}

jobject Deserializer::convert2JSignedMessage(JNIEnv *env, const std::string &address,
                                             const std::string &signature,
                                             const std::string &rfc2440) {
    syslog(LOG_DEBUG, "[JNI] convert2JSignedMessage()");
    jclass clazz = env->FindClass("com/nunchuk/android/model/SignedMessage");
    jmethodID constructor = env->GetMethodID(clazz, "<init>", "()V");
    jobject instance = env->NewObject(clazz, constructor);
    try {
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setAddress",
                                                       "(Ljava/lang/String;)V"),
                            env->NewStringUTF(address.c_str()));
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setSignature",
                                                       "(Ljava/lang/String;)V"),
                            env->NewStringUTF(signature.c_str()));
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setRfc2440",
                                                       "(Ljava/lang/String;)V"),
                            env->NewStringUTF(rfc2440.c_str()));
    } catch (const std::exception &e) {
        syslog(LOG_DEBUG, "[JNI] convert2JSignedMessage error::%s", e.what());
    }
    return instance;
}

jobject Deserializer::convert2JColdCardHealth(JNIEnv *env, const HealthStatus &status,
                                              const std::string signature) {
    jclass clazz = env->FindClass("com/nunchuk/android/model/ColdCardHealth");
    jmethodID constructor = env->GetMethodID(clazz, "<init>", "()V");
    jobject instance = env->NewObject(clazz, constructor);
    try {
        env->CallVoidMethod(instance,
                            env->GetMethodID(clazz, "setSignature", "(Ljava/lang/String;)V"),
                            env->NewStringUTF(signature.c_str()));
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setStatus",
                                                       "(Lcom/nunchuk/android/type/HealthStatus;)V"),
                            convert2JHealthStatus(env, status));
    } catch (const std::exception &e) {
        syslog(LOG_DEBUG, "[JNI] convert2JColdCardHealth error::%s", e.what());
    }
    return instance;
}

jobject Deserializer::convert2JSignerTag(JNIEnv *env, const SignerTag &tag) {
    jstring className = env->NewStringUTF("com/nunchuk/android/type/SignerTagHelper");
    jclass clazz = static_cast<jclass>(env->CallObjectMethod(Initializer::get()->classLoader,
                                                             Initializer::get()->loadClassMethod,
                                                             className));
    env->DeleteLocalRef(className);
    jmethodID staticMethod = env->GetStaticMethodID(clazz, "from",
                                                    "(I)Lcom/nunchuk/android/type/SignerTag;");
    return env->CallStaticObjectMethod(clazz, staticMethod, (int) tag);
}

jobject Deserializer::convert2JSignerTags(JNIEnv *env, const std::vector<SignerTag> &tags) {
    syslog(LOG_DEBUG, "[JNI] convert2JSignerTags()");
    static auto arrayListClass = static_cast<jclass>(env->NewGlobalRef(
            env->FindClass("java/util/ArrayList")));
    static jmethodID constructor = env->GetMethodID(arrayListClass, "<init>", "()V");
    jmethodID addMethod = env->GetMethodID(arrayListClass, "add", "(Ljava/lang/Object;)Z");
    jobject arrayListInstance = env->NewObject(arrayListClass, constructor);
    for (const SignerTag &s: tags) {
        jobject element = convert2JSignerTag(env, s);
        env->CallBooleanMethod(arrayListInstance, addMethod, element);
        env->DeleteLocalRef(element);
    }
    return arrayListInstance;
}

jobject Deserializer::convert2JInt(JNIEnv *env, const int value) {
    jclass clazz = env->FindClass("java/lang/Integer");
    jmethodID staticMethod = env->GetStaticMethodID(clazz, "valueOf", "(I)Ljava/lang/Integer;");
    return env->CallStaticObjectMethod(clazz, staticMethod, value);
}

jobject Deserializer::convert2JInts(JNIEnv *env, const std::vector<int> &value) {
    syslog(LOG_DEBUG, "[JNI] convert2JSignerTags()");
    static auto setClass = static_cast<jclass>(env->NewGlobalRef(
            env->FindClass("java/util/HashSet")));
    static jmethodID constructor = env->GetMethodID(setClass, "<init>", "()V");
    jmethodID addMethod = env->GetMethodID(setClass, "add", "(Ljava/lang/Object;)Z");
    jobject setInstance = env->NewObject(setClass, constructor);
    for (auto i: value) {
        env->CallBooleanMethod(setInstance, addMethod, convert2JInt(env, i));
    }
    return setInstance;
}

jobject Deserializer::convert2JLong(JNIEnv *env, const int64_t value) {
    jclass clazz = env->FindClass("java/lang/Long");
    jmethodID staticMethod = env->GetStaticMethodID(clazz, "valueOf", "(J)Ljava/lang/Long;");
    return env->CallStaticObjectMethod(clazz, staticMethod, value);
}

jobject Deserializer::convert2JLongs(JNIEnv *env, const std::vector<int64_t> &value) {
    syslog(LOG_DEBUG, "[JNI] convert2JLongs()");
    static auto listClass = static_cast<jclass>(env->NewGlobalRef(
            env->FindClass("java/util/ArrayList")));
    static jmethodID constructor = env->GetMethodID(listClass, "<init>", "()V");
    jmethodID addMethod = env->GetMethodID(listClass, "add", "(Ljava/lang/Object;)Z");
    jobject listInstance = env->NewObject(listClass, constructor);
    for (auto i: value) {
        env->CallBooleanMethod(listInstance, addMethod, convert2JLong(env, i));
    }
    return listInstance;
}

jobject Deserializer::convert2JCoinTag(JNIEnv *env, const CoinTag &tag) {
    syslog(LOG_DEBUG, "[JNI] convert2JCoinTag()");
    jclass clazz = env->FindClass("com/nunchuk/android/model/CoinTag");
    jmethodID constructor = env->GetMethodID(clazz, "<init>", "()V");
    jobject instance = env->NewObject(clazz, constructor);
    try {
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setId", "(I)V"), tag.get_id());
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setName", "(Ljava/lang/String;)V"),
                            env->NewStringUTF(tag.get_name().c_str()));
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setColor", "(Ljava/lang/String;)V"),
                            env->NewStringUTF(tag.get_color().c_str()));
    } catch (const std::exception &e) {
        syslog(LOG_DEBUG, "[JNI] convert2JCoinTag error::%s", e.what());
    }
    return instance;
}

jobject Deserializer::convert2JCoinTags(JNIEnv *env, const std::vector<CoinTag> &tags) {
    syslog(LOG_DEBUG, "[JNI] convert2JCoinTags()");
    static auto arrayListClass = static_cast<jclass>(env->NewGlobalRef(
            env->FindClass("java/util/ArrayList")));
    static jmethodID constructor = env->GetMethodID(arrayListClass, "<init>", "()V");
    jmethodID addMethod = env->GetMethodID(arrayListClass, "add", "(Ljava/lang/Object;)Z");
    jobject arrayListInstance = env->NewObject(arrayListClass, constructor);
    for (const CoinTag &s: tags) {
        jobject element = convert2JCoinTag(env, s);
        env->CallBooleanMethod(arrayListInstance, addMethod, element);
        env->DeleteLocalRef(element);
    }
    return arrayListInstance;
}

jobject Deserializer::convert2JCoinCollection(JNIEnv *env, const CoinCollection &collection) {
    syslog(LOG_DEBUG, "[JNI] convert2JCoinCollection()");
    jclass clazz = env->FindClass("com/nunchuk/android/model/CoinCollection");
    jmethodID constructor = env->GetMethodID(clazz, "<init>", "()V");
    jobject instance = env->NewObject(clazz, constructor);
    try {
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setId", "(I)V"),
                            collection.get_id());
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setName", "(Ljava/lang/String;)V"),
                            env->NewStringUTF(collection.get_name().c_str()));
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setAddNewCoin", "(Z)V"),
                            collection.is_add_new_coin());
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setAutoLock", "(Z)V"),
                            collection.is_auto_lock());
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setTagIds", "(Ljava/util/Set;)V"),
                            convert2JInts(env, collection.get_add_coins_with_tag()));
    } catch (const std::exception &e) {
        syslog(LOG_DEBUG, "[JNI] convert2JCoinCollection error::%s", e.what());
    }
    return instance;
}

jobject Deserializer::convert2JCoinCollections(JNIEnv *env,
                                               const std::vector<CoinCollection> &collections) {
    syslog(LOG_DEBUG, "[JNI] convert2JCoinCollections()");
    static auto arrayListClass = static_cast<jclass>(env->NewGlobalRef(
            env->FindClass("java/util/ArrayList")));
    static jmethodID constructor = env->GetMethodID(arrayListClass, "<init>", "()V");
    jmethodID addMethod = env->GetMethodID(arrayListClass, "add", "(Ljava/lang/Object;)Z");
    jobject arrayListInstance = env->NewObject(arrayListClass, constructor);
    for (const CoinCollection &s: collections) {
        jobject element = convert2JCoinCollection(env, s);
        env->CallBooleanMethod(arrayListInstance, addMethod, element);
        env->DeleteLocalRef(element);
    }
    return arrayListInstance;
}

jobject
Deserializer::convert2JCollectionUnspentOutputs(JNIEnv *env,
                                                const std::vector<std::vector<UnspentOutput>> &outputs) {
    static auto arrayListClass = static_cast<jclass>(env->NewGlobalRef(
            env->FindClass("java/util/ArrayList")));
    static jmethodID constructor = env->GetMethodID(arrayListClass, "<init>", "()V");
    jmethodID addMethod = env->GetMethodID(arrayListClass, "add", "(Ljava/lang/Object;)Z");
    jobject arrayListInstance = env->NewObject(arrayListClass, constructor);
    for (const std::vector<UnspentOutput> &output: outputs) {
        jobject element = convert2JUnspentOutputs(env, output);
        env->CallBooleanMethod(arrayListInstance, addMethod, element);
        env->DeleteLocalRef(element);
    }
    return arrayListInstance;
}

jobject Deserializer::convert2JPairAmount(JNIEnv *env, const std::pair<Amount, Amount> &pair) {
    syslog(LOG_DEBUG, "[JNI] convert2JPairAmount()");
    jclass clazz = env->FindClass("com/nunchuk/android/model/PairAmount");
    jmethodID constructor = env->GetMethodID(clazz, "<init>", "()V");
    jobject instance = env->NewObject(clazz, constructor);
    try {
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setFirst",
                                                       "(Lcom/nunchuk/android/model/Amount;)V"),
                            convert2JAmount(env, pair.first));
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setSecond",
                                                       "(Lcom/nunchuk/android/model/Amount;)V"),
                            convert2JAmount(env, pair.second));
    } catch (const std::exception &e) {
        syslog(LOG_DEBUG, "[JNI] convert2JPairAmount error::%s", e.what());
    }
    return instance;
}

jobject Deserializer::convert2JDraftRollOverTransaction(JNIEnv *env, const Transaction &transaction,
                                                        const std::vector<int> &tagIds,
                                                        const std::vector<int> &collectionIds) {
    syslog(LOG_DEBUG, "[JNI] convert2JDraftRollOverTransaction()");
    jclass clazz = env->FindClass("com/nunchuk/android/model/DraftRollOverTransaction");
    jmethodID constructor = env->GetMethodID(clazz, "<init>", "()V");
    jobject instance = env->NewObject(clazz, constructor);
    try {
        jobject element = convert2JTransaction(env, transaction);
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setTransaction",
                                                       "(Lcom/nunchuk/android/model/Transaction;)V"),
                            element);
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setTagIds", "(Ljava/util/Set;)V"),
                            convert2JInts(env, tagIds));
        env->CallVoidMethod(instance,
                            env->GetMethodID(clazz, "setCollectionIds", "(Ljava/util/Set;)V"),
                            convert2JInts(env, collectionIds));
    } catch (const std::exception &e) {
        syslog(LOG_DEBUG, "[JNI] convert2JDraftRollOverTransaction error::%s", e.what());
    }
    return instance;
}

jobjectArray Deserializer::convert2JDraftRollOverTransactions(JNIEnv *env,
                                                              const std::map<std::pair<std::set<int>, std::set<int>>, Transaction> txs) {
    syslog(LOG_DEBUG, "[JNI] convert2JDraftRollOverTransactions()");
    jclass clazz = env->FindClass("com/nunchuk/android/model/DraftRollOverTransaction");
    jobjectArray ret = env->NewObjectArray(txs.size(), clazz, nullptr);
    int i = 0;
    for (const auto &tx: txs) {
        jobject element = convert2JDraftRollOverTransaction(env, tx.second,
                                                            std::vector<int>(tx.first.first.begin(),
                                                                             tx.first.first.end()),
                                                            std::vector<int>(
                                                                    tx.first.second.begin(),
                                                                    tx.first.second.end()));
        env->SetObjectArrayElement(ret, i++, element);
    }
    return ret;
}

jobject Deserializer::convert2JGroupSandbox(JNIEnv *env, const GroupSandbox &sandbox) {
    syslog(LOG_DEBUG, "[JNI] convert2JGroupSandbox()");
    jstring className = env->NewStringUTF("com/nunchuk/android/model/GroupSandbox");
    jclass clazz = reinterpret_cast<jclass>(env->CallObjectMethod(Initializer::get()->classLoader,
                                                                  Initializer::get()->loadClassMethod,
                                                                  className));
    env->DeleteLocalRef(className);
    syslog(LOG_DEBUG, "[JNI] name::%s", sandbox.get_name().c_str());
    syslog(LOG_DEBUG, "[JNI] id::%s", sandbox.get_id().c_str());

    jmethodID constructor = env->GetMethodID(clazz, "<init>",
                                             "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;IILcom/nunchuk/android/type/AddressType;Ljava/util/List;ZLjava/lang/String;Ljava/util/List;Lcom/nunchuk/android/type/WalletType;Ljava/lang/String;Ljava/util/Map;Ljava/util/Map;)V");

    className = env->NewStringUTF("com/nunchuk/android/model/OccupiedSlot");
    jclass occupiedSlotClass = reinterpret_cast<jclass>(env->CallObjectMethod(
            Initializer::get()->classLoader,
            Initializer::get()->loadClassMethod,
            className));
    env->DeleteLocalRef(className);

    jmethodID occupiedSlotConstructor = env->GetMethodID(occupiedSlotClass, "<init>",
                                                         "(JLjava/lang/String;)V");

    jclass arrayListClass = env->FindClass("java/util/ArrayList");
    jmethodID arrayListConstructor = env->GetMethodID(arrayListClass, "<init>", "()V");
    jobject occupiedSlots = env->NewObject(arrayListClass, arrayListConstructor);
    jmethodID addMethod = env->GetMethodID(arrayListClass, "add", "(ILjava/lang/Object;)V");

    for (int i = 0; i < sandbox.get_signers().size(); i++) {
        env->CallVoidMethod(occupiedSlots, addMethod, i, nullptr);
    }

    for (int index = 0; index < sandbox.get_signers().size(); index++) {
        if (sandbox.get_signers()[index].get_master_fingerprint().empty()) {
            if (sandbox.get_occupied().contains(index)) {
                std::pair<time_t, std::string> ts_uid = sandbox.get_occupied().at(index);
                jobject occupiedSlot = env->NewObject(occupiedSlotClass, occupiedSlotConstructor,
                                                      static_cast<jlong>(ts_uid.first),
                                                      env->NewStringUTF(ts_uid.second.c_str()));
                env->CallVoidMethod(occupiedSlots, addMethod, index, occupiedSlot);
                env->DeleteLocalRef(occupiedSlot);
            }
        }
    }
    // Check if miniscript template is not empty before calling get_named_signers and get_named_occupied
    std::string miniscript_template = sandbox.get_miniscript_template();
    jobject namedSignersMap = nullptr;
    jobject namedOccupiedMap = nullptr;
    
    if (!miniscript_template.empty()) {
        try {
            namedSignersMap = convert2JNamedSignersMap(env, sandbox.get_named_signers());
            namedOccupiedMap = convert2JNamedOccupiedMap(env, sandbox.get_named_occupied());
        } catch (const std::exception &e) {
            syslog(LOG_DEBUG, "[JNI] convert2JGroupSandbox error getting named data::%s", e.what());
            // Create empty maps as fallback
            namedSignersMap = convert2JNamedSignersMap(env, std::map<std::string, SingleSigner>{});
            namedOccupiedMap = convert2JNamedOccupiedMap(env, std::map<std::string, std::pair<time_t, std::string>>{});
        }
    } else {
        // Create empty maps when miniscript template is empty
        namedSignersMap = convert2JNamedSignersMap(env, std::map<std::string, SingleSigner>{});
        namedOccupiedMap = convert2JNamedOccupiedMap(env, std::map<std::string, std::pair<time_t, std::string>>{});
    }
    
    jobject instance = env->NewObject(
            clazz,
            constructor,
            env->NewStringUTF(sandbox.get_id().c_str()),
            env->NewStringUTF(sandbox.get_replace_wallet_id().c_str()),
            env->NewStringUTF(sandbox.get_name().c_str()),
            env->NewStringUTF(sandbox.get_url().c_str()),
            sandbox.get_m(),
            sandbox.get_n(),
            convert2JAddressType(env, sandbox.get_address_type()),
            convert2JSigners(env, sandbox.get_signers()),
            sandbox.is_finalized(),
            env->NewStringUTF(sandbox.get_wallet_id().c_str()),
            occupiedSlots,
            convert2JWalletType(env, sandbox.get_wallet_type()),
            env->NewStringUTF(miniscript_template.c_str()),
            namedSignersMap,
            namedOccupiedMap
    );
    return instance;
}

jobject Deserializer::convert2JFreeGroupConfig(JNIEnv *env, const GroupConfig &config,
                                               const AddressType &addressType) {
    syslog(LOG_DEBUG, "[JNI] convert2JFreeGroupConfig()");
    jclass clazz = env->FindClass("com/nunchuk/android/model/GlobalGroupWalletConfig");
    jmethodID constructor = env->GetMethodID(clazz, "<init>", "()V");
    jobject instance = env->NewObject(clazz, constructor);
    try {

        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setTotal", "(I)V"),
                            config.get_total());
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setRemain", "(I)V"),
                            config.get_remain());
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setMaxKey", "(I)V"),
                            config.get_max_keys(addressType));
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setRetentionDaysOptions",
                                                       "(Ljava/util/Set;)V"),
                            convert2JInts(env, config.get_retention_days_options()));
    } catch (const std::exception &e) {
        syslog(LOG_DEBUG, "[JNI] convert2JFreeGroupWalletConfig error::%s", e.what());
    }
    return instance;
}

jobject
Deserializer::convert2JGroupsSandbox(JNIEnv *env, const std::vector<GroupSandbox> &groupSandbox) {
    syslog(LOG_DEBUG, "[JNI] convert2JGroupsSandbox()");
    jclass arrayListClass = env->FindClass("java/util/ArrayList");
    jmethodID arrayListConstructor = env->GetMethodID(arrayListClass, "<init>", "()V");
    jobject arrayList = env->NewObject(arrayListClass, arrayListConstructor);
    jmethodID arrayListAdd = env->GetMethodID(arrayListClass, "add", "(Ljava/lang/Object;)Z");

    for (const auto &group: groupSandbox) {
        jobject jGroup = convert2JGroupSandbox(env, group);
        env->CallBooleanMethod(arrayList, arrayListAdd, jGroup);
        env->DeleteLocalRef(jGroup);
    }

    return arrayList;
}

jobject Deserializer::convert2JGroupMessage(JNIEnv *env, const nunchuk::GroupMessage &message) {
    syslog(LOG_DEBUG, "[JNI] convert2JGroupMessage()");
    jstring className = env->NewStringUTF("com/nunchuk/android/model/FreeGroupMessage");
    jclass clazz = static_cast<jclass>(env->CallObjectMethod(Initializer::get()->classLoader,
                                                             Initializer::get()->loadClassMethod,
                                                             className));
    env->DeleteLocalRef(className);
    jmethodID constructor = env->GetMethodID(clazz, "<init>", "()V");
    jobject instance = env->NewObject(clazz, constructor);
    try {
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setId", "(Ljava/lang/String;)V"),
                            env->NewStringUTF(message.get_id().c_str()));
        env->CallVoidMethod(instance,
                            env->GetMethodID(clazz, "setWalletId", "(Ljava/lang/String;)V"),
                            env->NewStringUTF(message.get_wallet_id().c_str()));
        env->CallVoidMethod(instance,
                            env->GetMethodID(clazz, "setContent", "(Ljava/lang/String;)V"),
                            env->NewStringUTF(message.get_content().c_str()));
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setTimestamp", "(J)V"),
                            message.get_ts());
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setSender", "(Ljava/lang/String;)V"),
                            env->NewStringUTF(message.get_sender().c_str()));
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setSigner", "(Ljava/lang/String;)V"),
                            env->NewStringUTF(message.get_signer().c_str()));
    } catch (const std::exception &e) {
        syslog(LOG_DEBUG, "[JNI] convert2JGroupMessage error::%s", e.what());
    }
    return instance;
}

jobject Deserializer::convert2JGroupMessages(JNIEnv *env,
                                             const std::vector<nunchuk::GroupMessage> &messages) {
    static auto arrayListClass = static_cast<jclass>(env->NewGlobalRef(
            env->FindClass("java/util/ArrayList")));
    static jmethodID constructor = env->GetMethodID(arrayListClass, "<init>", "()V");
    jmethodID addMethod = env->GetMethodID(arrayListClass, "add", "(Ljava/lang/Object;)Z");
    jobject arrayListInstance = env->NewObject(arrayListClass, constructor);
    for (const GroupMessage &message: messages) {
        jobject element = convert2JGroupMessage(env, message);
        env->CallBooleanMethod(arrayListInstance, addMethod, element);
        env->DeleteLocalRef(element);
    }
    return arrayListInstance;
}

jobject Deserializer::convert2JGroupWalletConfig(JNIEnv *env, const GroupWalletConfig &config) {
    syslog(LOG_DEBUG, "[JNI] convert2JGroupWalletConfig()");
    jclass clazz = env->FindClass("com/nunchuk/android/model/FreeGroupWalletConfig");
    jmethodID constructor = env->GetMethodID(clazz, "<init>", "()V");
    jobject instance = env->NewObject(clazz, constructor);
    try {
        env->CallVoidMethod(instance, env->GetMethodID(clazz, "setChatRetentionDays", "(I)V"),
                            config.get_chat_retention_days());
    } catch (const std::exception &e) {
        syslog(LOG_DEBUG, "[JNI] convert2JGroupWalletConfig error::%s", e.what());
    }
    return instance;
}

jobject Deserializer::convert2JWalletTemplate(JNIEnv *env, const WalletTemplate &wallet_template) {
    syslog(LOG_DEBUG, "[JNI] convert2JWalletTemplate()");
    jclass clazz = Initializer::get()->getClass(env, "com/nunchuk.android.type.WalletTemplate");
    jmethodID staticMethod = env->GetStaticMethodID(clazz, "from",
                                                    "(I)Lcom/nunchuk/android/type/WalletTemplate;");
    return env->CallStaticObjectMethod(clazz, staticMethod, (int) wallet_template);
}

jobject Deserializer::convert2JScriptNode(JNIEnv *env, const ScriptNode &node) {
    syslog(LOG_DEBUG, "[JNI] convert2JScriptNode()");
    jstring className = env->NewStringUTF("com/nunchuk/android/model/ScriptNode");
    jclass clazz = reinterpret_cast<jclass>(env->CallObjectMethod(Initializer::get()->classLoader,
                                                                  Initializer::get()->loadClassMethod,
                                                                  className));
    // Updated constructor signature to include TimeLock: (Ljava/util/List;Ljava/lang/String;Ljava/util/List;Ljava/util/List;I[BLcom/nunchuk/android/model/TimeLock;)V
    jmethodID constructor = env->GetMethodID(clazz,
                                             "<init>",
                                             "(Ljava/util/List;Ljava/lang/String;Ljava/util/List;Ljava/util/List;I[BLcom/nunchuk/android/model/TimeLock;)V");

    // Map ScriptNodeId to List<Integer>
    jclass arrayListClass = env->FindClass("java/util/ArrayList");
    jmethodID arrayListConstructor = env->GetMethodID(arrayListClass, "<init>", "()V");
    jmethodID arrayListAdd = env->GetMethodID(arrayListClass, "add", "(Ljava/lang/Object;)Z");
    jobject idList = env->NewObject(arrayListClass, arrayListConstructor);
    for (size_t idx : node.get_id()) {
        jobject integerObj = env->NewObject(env->FindClass("java/lang/Integer"), env->GetMethodID(env->FindClass("java/lang/Integer"), "<init>", "(I)V"), static_cast<jint>(idx));
        env->CallBooleanMethod(idList, arrayListAdd, integerObj);
        env->DeleteLocalRef(integerObj);
    }

    jstring typeStr = env->NewStringUTF(ScriptNode::type_to_string(node.get_type()).c_str());
    jobject keysList = convert2JListString(env, node.get_keys());
    jobject subsList = convert2JScriptNodes(env, node.get_subs());
    if (subsList == nullptr) {
        // create an empty list to avoid null pointer in Kotlin
        jclass arrayListClass2 = env->FindClass("java/util/ArrayList");
        jmethodID constructorArr = env->GetMethodID(arrayListClass2, "<init>", "()V");
        subsList = env->NewObject(arrayListClass2, constructorArr);
    }
    jint kValue = static_cast<jint>(node.get_k());
    const std::vector<unsigned char>& dataVec = node.get_data();
    jbyteArray dataArray = env->NewByteArray(dataVec.size());
    if (!dataVec.empty()) {
        env->SetByteArrayRegion(dataArray, 0, dataVec.size(), reinterpret_cast<const jbyte*>(dataVec.data()));
    }

    // Extract timelock information for AFTER and OLDER types
    jobject timeLockObj = nullptr;
    if (node.get_type() == ScriptNode::Type::AFTER) {
        Timelock timelock = Timelock::FromK(true, node.get_k());
        timeLockObj = convert2JTimeLock(env, timelock);
    } else if (node.get_type() == ScriptNode::Type::OLDER) {
        Timelock timelock = Timelock::FromK(false, node.get_k());
        timeLockObj = convert2JTimeLock(env, timelock);
    }

    // Construct Kotlin ScriptNode object
    jobject instance = env->NewObject(clazz, constructor, idList, typeStr, keysList, subsList, kValue, dataArray, timeLockObj);
    // Cleanup
    env->DeleteLocalRef(typeStr);
    env->DeleteLocalRef(dataArray);
    env->DeleteLocalRef(idList);
    if (timeLockObj) {
        env->DeleteLocalRef(timeLockObj);
    }
    return instance;
}

jobject Deserializer::convert2JScriptNodes(JNIEnv *env, const std::vector<ScriptNode> &nodes) {
    static auto arrayListClass = static_cast<jclass>(env->NewGlobalRef(
            env->FindClass("java/util/ArrayList")));
    static jmethodID constructor = env->GetMethodID(arrayListClass, "<init>", "()V");
    jmethodID addMethod = env->GetMethodID(arrayListClass, "add", "(Ljava/lang/Object;)Z");
    jobject arrayListInstance = env->NewObject(arrayListClass, constructor);

    for (const ScriptNode &node : nodes) {
        jobject element = convert2JScriptNode(env, node);
        env->CallBooleanMethod(arrayListInstance, addMethod, element);
        env->DeleteLocalRef(element);
    }

    return arrayListInstance;
}

jobject Deserializer::convert2JScriptNodeResult(JNIEnv *env, const ScriptNode &node, const std::vector<std::string> &keyPath) {
    syslog(LOG_DEBUG, "[JNI] convert2JScriptNodeResult()");
    jclass clazz = env->FindClass("com/nunchuk/android/model/ScriptNodeResult");
    jmethodID constructor = env->GetMethodID(clazz, "<init>", "(Lcom/nunchuk/android/model/ScriptNode;Ljava/util/List;)V");

    jobject scriptNodeObj = convert2JScriptNode(env, node);
    jobject keyPathList = convert2JListString(env, keyPath);

    jobject instance = env->NewObject(clazz, constructor, scriptNodeObj, keyPathList);

    // Cleanup
    env->DeleteLocalRef(scriptNodeObj);
    env->DeleteLocalRef(keyPathList);

    return instance;
}

jobject Deserializer::convert2JMiniscriptTemplateResult(JNIEnv *env, const std::string &template_str, bool isValidTapscript, bool isValidPolicy, bool isValidMiniscriptTemplate) {
    syslog(LOG_DEBUG, "[JNI] convert2JMiniscriptTemplateResult()");
    jclass clazz = env->FindClass("com/nunchuk/android/model/MiniscriptTemplateResult");
    jmethodID constructor = env->GetMethodID(clazz, "<init>", "(Ljava/lang/String;ZZZ)V");
    jobject instance = env->NewObject(clazz, constructor, env->NewStringUTF(template_str.c_str()), isValidTapscript, isValidPolicy, isValidMiniscriptTemplate);
    return instance;
}

jobject Deserializer::convert2JMiniscriptTimelockBased(JNIEnv *env, nunchuk::Timelock::Based based) {
    jstring className = env->NewStringUTF("com/nunchuk/android/type/MiniscriptTimelockBasedHelper");
    jclass clazz = static_cast<jclass>(env->CallObjectMethod(Initializer::get()->classLoader,
                                                             Initializer::get()->loadClassMethod,
                                                             className));
    env->DeleteLocalRef(className);
    jmethodID staticMethod = env->GetStaticMethodID(clazz, "from", "(I)Lcom/nunchuk/android/type/MiniscriptTimelockBased;");
    return env->CallStaticObjectMethod(clazz, staticMethod, static_cast<jint>(based));
}

jobject Deserializer::convert2JSigningPath(JNIEnv *env, const SigningPath &signingPath) {
    jclass signingPathClass = env->FindClass("com/nunchuk/android/model/SigningPath");
    jmethodID constructor = env->GetMethodID(signingPathClass, "<init>", "(Ljava/util/List;)V");
    jclass arrayListClass = env->FindClass("java/util/ArrayList");
    jmethodID arrayListConstructor = env->GetMethodID(arrayListClass, "<init>", "()V");
    jmethodID arrayListAdd = env->GetMethodID(arrayListClass, "add", "(Ljava/lang/Object;)Z");
    jobject outerList = env->NewObject(arrayListClass, arrayListConstructor);
    for (const auto &scriptNodeId : signingPath) {
        jobject innerList = env->NewObject(arrayListClass, arrayListConstructor);
        for (size_t idx : scriptNodeId) {
            jobject integerObj = env->NewObject(env->FindClass("java/lang/Integer"), env->GetMethodID(env->FindClass("java/lang/Integer"), "<init>", "(I)V"), static_cast<jint>(idx));
            env->CallBooleanMethod(innerList, arrayListAdd, integerObj);
            env->DeleteLocalRef(integerObj);
        }
        env->CallBooleanMethod(outerList, arrayListAdd, innerList);
        env->DeleteLocalRef(innerList);
    }
    jobject signingPathObj = env->NewObject(signingPathClass, constructor, outerList);
    env->DeleteLocalRef(outerList);
    return signingPathObj;
}

jobject Deserializer::convert2JSigningPathAmountPairs(JNIEnv *env, const std::vector<std::pair<SigningPath, Amount>> &pairs) {
    jclass arrayListClass = env->FindClass("java/util/ArrayList");
    jmethodID arrayListConstructor = env->GetMethodID(arrayListClass, "<init>", "()V");
    jmethodID arrayListAdd = env->GetMethodID(arrayListClass, "add", "(Ljava/lang/Object;)Z");
    jobject arrayList = env->NewObject(arrayListClass, arrayListConstructor);
    
    // Create Pair class using the correct class path for Kotlin
    jstring className = env->NewStringUTF("kotlin/Pair");
    jclass pairClass = static_cast<jclass>(env->CallObjectMethod(Initializer::get()->classLoader,
                                                                 Initializer::get()->loadClassMethod,
                                                                 className));
    env->DeleteLocalRef(className);
    
    if (pairClass == nullptr) {
        syslog(LOG_DEBUG, "[JNI] convert2JSigningPathAmountPairs: Could not find kotlin.Pair class");
        return arrayList; // Return empty list
    }
    
    jmethodID pairConstructor = env->GetMethodID(pairClass, "<init>", "(Ljava/lang/Object;Ljava/lang/Object;)V");
    if (pairConstructor == nullptr) {
        syslog(LOG_DEBUG, "[JNI] convert2JSigningPathAmountPairs: Could not find Pair constructor");
        return arrayList; // Return empty list
    }
    
    for (const auto &pair : pairs) {
        jobject signingPathObj = convert2JSigningPath(env, pair.first);
        jobject amountObj = convert2JAmount(env, pair.second);
        jobject pairObj = env->NewObject(pairClass, pairConstructor, signingPathObj, amountObj);
        env->CallBooleanMethod(arrayList, arrayListAdd, pairObj);
        env->DeleteLocalRef(signingPathObj);
        env->DeleteLocalRef(amountObj);
        env->DeleteLocalRef(pairObj);
    }
    return arrayList;
}

jobject Deserializer::convert2JMiniscriptTimelockType(JNIEnv *env, nunchuk::Timelock::Type type) {
    jstring className = env->NewStringUTF("com/nunchuk/android/type/MiniscriptTimelockTypeHelper");
    jclass clazz = static_cast<jclass>(env->CallObjectMethod(Initializer::get()->classLoader,
                                                             Initializer::get()->loadClassMethod,
                                                             className));
    env->DeleteLocalRef(className);
    jmethodID staticMethod = env->GetStaticMethodID(clazz, "from", "(I)Lcom/nunchuk/android/type/MiniscriptTimelockType;");
    return env->CallStaticObjectMethod(clazz, staticMethod, static_cast<jint>(type));
}

jobject Deserializer::convert2JTimeLock(JNIEnv *env, const nunchuk::Timelock &timeLock) {
    syslog(LOG_DEBUG, "[JNI] convert2JTimeLock()");
    jclass clazz = env->FindClass("com/nunchuk/android/model/TimeLock");
    jmethodID constructor = env->GetMethodID(clazz, "<init>", "(Lcom/nunchuk/android/type/MiniscriptTimelockBased;Lcom/nunchuk/android/type/MiniscriptTimelockType;J)V");

    jobject basedObj = convert2JMiniscriptTimelockBased(env, timeLock.based());
    jobject typeObj = convert2JMiniscriptTimelockType(env, timeLock.type());
    jlong valueVal = static_cast<jlong>(timeLock.value());

    jobject instance = env->NewObject(clazz, constructor, basedObj, typeObj, valueVal);

    // Cleanup
    env->DeleteLocalRef(basedObj);
    env->DeleteLocalRef(typeObj);

    return instance;
}

jobject Deserializer::convert2JPairLongMiniscriptTimelockBased(JNIEnv *env, const std::pair<int64_t, nunchuk::Timelock::Based> &pair) {
    jclass pairClass = env->FindClass("kotlin/Pair");
    jmethodID pairConstructor = env->GetMethodID(pairClass, "<init>", "(Ljava/lang/Object;Ljava/lang/Object;)V");

    // Create Long object for the first element
    jclass longClass = env->FindClass("java/lang/Long");
    jmethodID longConstructor = env->GetMethodID(longClass, "<init>", "(J)V");
    jobject longObj = env->NewObject(longClass, longConstructor, static_cast<jlong>(pair.first));

    // Create MiniscriptTimelockBased object for the second element
    jobject timelockBasedObj = convert2JMiniscriptTimelockBased(env, pair.second);

    // Create Pair object
    jobject pairObj = env->NewObject(pairClass, pairConstructor, longObj, timelockBasedObj);

    // Cleanup
    env->DeleteLocalRef(longObj);
    env->DeleteLocalRef(timelockBasedObj);

    return pairObj;
}

jobject Deserializer::convert2JNamedSignersMap(JNIEnv *env, const std::map<std::string, SingleSigner> &namedSigners) {
    syslog(LOG_DEBUG, "[JNI] convert2JNamedSignersMap()");
    jclass hashMapClass = env->FindClass("java/util/HashMap");
    jmethodID hashMapConstructor = env->GetMethodID(hashMapClass, "<init>", "()V");
    jobject hashMap = env->NewObject(hashMapClass, hashMapConstructor);
    jmethodID putMethod = env->GetMethodID(hashMapClass, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");

    for (const auto& pair : namedSigners) {
        jobject key = env->NewStringUTF(pair.first.c_str());
        jobject value = convert2JSigner(env, pair.second);
        env->CallObjectMethod(hashMap, putMethod, key, value);
        env->DeleteLocalRef(key);
        env->DeleteLocalRef(value);
    }
    return hashMap;
}

jobject Deserializer::convert2JNamedOccupiedMap(JNIEnv *env, const std::map<std::string, std::pair<time_t, std::string>> &namedOccupied) {
    syslog(LOG_DEBUG, "[JNI] convert2JNamedOccupiedMap()");
    jclass hashMapClass = env->FindClass("java/util/HashMap");
    jmethodID hashMapConstructor = env->GetMethodID(hashMapClass, "<init>", "()V");
    jobject hashMap = env->NewObject(hashMapClass, hashMapConstructor);
    jmethodID putMethod = env->GetMethodID(hashMapClass, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");

    // Create Pair class using the correct class path for Kotlin
    jstring className = env->NewStringUTF("kotlin/Pair");
    jclass pairClass = static_cast<jclass>(env->CallObjectMethod(Initializer::get()->classLoader,
                                                                 Initializer::get()->loadClassMethod,
                                                                 className));
    env->DeleteLocalRef(className);
    
    if (pairClass == nullptr) {
        syslog(LOG_DEBUG, "[JNI] convert2JNamedOccupiedMap: Could not find kotlin.Pair class");
        return hashMap; // Return empty map
    }
    
    jmethodID pairConstructor = env->GetMethodID(pairClass, "<init>", "(Ljava/lang/Object;Ljava/lang/Object;)V");
    if (pairConstructor == nullptr) {
        syslog(LOG_DEBUG, "[JNI] convert2JNamedOccupiedMap: Could not find Pair constructor");
        return hashMap; // Return empty map
    }

    for (const auto& pair : namedOccupied) {
        jobject key = env->NewStringUTF(pair.first.c_str());
        jobject first = convert2JLong(env, static_cast<int64_t>(pair.second.first));
        jobject second = env->NewStringUTF(pair.second.second.c_str());
        jobject pairObject = env->NewObject(pairClass, pairConstructor, first, second);
        env->CallObjectMethod(hashMap, putMethod, key, pairObject);
        env->DeleteLocalRef(key);
        env->DeleteLocalRef(first);
        env->DeleteLocalRef(second);
        env->DeleteLocalRef(pairObject);
    }
    return hashMap;
}

jobject Deserializer::convert2JKeySetStatusSingle(JNIEnv *env, const KeysetStatus &keySetStatus) {
    jclass keySetStatusClass = env->FindClass("com/nunchuk/android/model/KeySetStatus");
    jmethodID keySetStatusConstructor = env->GetMethodID(keySetStatusClass, "<init>", "()V");
    jobject keySetStatusInstance = env->NewObject(keySetStatusClass, keySetStatusConstructor);

    env->CallVoidMethod(keySetStatusInstance, env->GetMethodID(keySetStatusClass, "setStatus",
                                                               "(Lcom/nunchuk/android/type/TransactionStatus;)V"),
                        convert2JTransactionStatus(env, keySetStatus.first));
    env->CallVoidMethod(keySetStatusInstance,
                        env->GetMethodID(keySetStatusClass, "setSignerStatus",
                                         "(Ljava/util/Map;)V"),
                        convert2JStringBooleanMap(env, keySetStatus.second));
    return keySetStatusInstance;
}

jobject Deserializer::convert2JCoinsGroup(JNIEnv *env, const CoinsGroup &group) {
    // Resolve Kotlin CoinsGroup class and constructor
    jclass coinsGroupClass = env->FindClass("com/nunchuk/android/model/CoinsGroup");
    if (coinsGroupClass == nullptr) return nullptr;
    jmethodID ctor = env->GetMethodID(coinsGroupClass, "<init>", "(Ljava/util/List;JJ)V");
    if (ctor == nullptr) return nullptr;

    // Convert fields
    jobject jCoins = convert2JUnspentOutputs(env, group.first);
    jlong jStart = static_cast<jlong>(group.second.first);
    jlong jEnd = static_cast<jlong>(group.second.second);

    // Create Kotlin object
    return env->NewObject(coinsGroupClass, ctor, jCoins, jStart, jEnd);
}

jobject Deserializer::convert2JCoinsGroups(JNIEnv *env, const std::vector<CoinsGroup> &groups) {
    jclass arrayListClass = env->FindClass("java/util/ArrayList");
    jmethodID arrayListCtor = env->GetMethodID(arrayListClass, "<init>", "()V");
    jmethodID arrayListAdd = env->GetMethodID(arrayListClass, "add", "(Ljava/lang/Object;)Z");
    jobject list = env->NewObject(arrayListClass, arrayListCtor);

    for (const auto &g : groups) {
        jobject jg = convert2JCoinsGroup(env, g);
        env->CallBooleanMethod(list, arrayListAdd, jg);
    }
    return list;
}
