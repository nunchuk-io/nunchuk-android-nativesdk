#include <cstring>
#include <jni.h>
#include <nunchuk.h>
#include "nunchukprovider.h"
#include "serializer.h"
#include "deserializer.h"
#include "descriptor.h"
#include "string-wrapper.h"
#include "utils/enumconverter.hpp"
#include "nfc.h"
#include "utils/rfc2440.hpp"

using namespace nunchuk;

extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_createSigner(
        JNIEnv *env,
        jobject thiz,
        jstring name,
        jstring xpub,
        jstring public_key,
        jstring derivation_path,
        jstring master_fingerprint,
        jobject type,
        jobject tags
) {
    syslog(LOG_DEBUG, "[JNI] createSigner()");
    syslog(LOG_DEBUG, "[JNI] name::%s", env->GetStringUTFChars(name, JNI_FALSE));
    syslog(LOG_DEBUG, "[JNI] xpub::%s", env->GetStringUTFChars(xpub, JNI_FALSE));
    syslog(LOG_DEBUG, "[JNI] public_key::%s", env->GetStringUTFChars(public_key, JNI_FALSE));
    syslog(LOG_DEBUG, "[JNI] derivation_path::%s",
           env->GetStringUTFChars(derivation_path, JNI_FALSE));
    syslog(LOG_DEBUG, "[JNI] master_fingerprint::%s",
           env->GetStringUTFChars(master_fingerprint, JNI_FALSE));
    try {
        auto cTags = Serializer::convert2CSignerTags(env, tags);
        const SingleSigner &signer = NunchukProvider::get()->nu->CreateSigner(
                env->GetStringUTFChars(name, JNI_FALSE),
                env->GetStringUTFChars(xpub, JNI_FALSE),
                env->GetStringUTFChars(public_key, JNI_FALSE),
                env->GetStringUTFChars(derivation_path, JNI_FALSE),
                env->GetStringUTFChars(master_fingerprint, JNI_FALSE),
                Serializer::convert2CSignerType(env, type),
                cTags
        );
        return Deserializer::convert2JSigner(env, signer);
    } catch (BaseException &e) {
        syslog(LOG_DEBUG, "[JNI] createSigner error::%s", e.what());
        Deserializer::convert2JException(env, e);
        return env->ExceptionOccurred();
    }
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_parseKeystoneSigner(
        JNIEnv *env,
        jobject thiz,
        jstring qr_data
) {
    syslog(LOG_DEBUG, "[JNI] parseKeystoneSigner()");
    syslog(LOG_DEBUG, "[JNI] qr_data::%s", env->GetStringUTFChars(qr_data, JNI_FALSE));
    try {
        auto c_qr_data = StringWrapper(env, qr_data);
        auto signers = NunchukProvider::get()->nu->ParseQRSigners({c_qr_data});
        return Deserializer::convert2JSigner(env, signers[0]);
    } catch (BaseException &e) {
        syslog(LOG_DEBUG, "[JNI] parseKeystoneSigner error::%s", e.what());
        Deserializer::convert2JException(env, e);
        return env->ExceptionOccurred();
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return env->ExceptionOccurred();
    }
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_parsePassportSigners(
        JNIEnv *env,
        jobject thiz,
        jobject qr_data
) {
    syslog(LOG_DEBUG, "[JNI] parsePassportSigners()");
    try {
        auto values = NunchukProvider::get()->nu->ParseQRSigners(
                Serializer::convert2CListString(env, qr_data)
        );
        return Deserializer::convert2JSigners(env, values);
    } catch (BaseException &e) {
        syslog(LOG_DEBUG, "[JNI] parsePassportSigners error::%s", e.what());
        Deserializer::convert2JException(env, e);
        return env->ExceptionOccurred();
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return env->ExceptionOccurred();
    }
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_getRemoteSigners(
        JNIEnv *env,
        jobject thiz
) {
    syslog(LOG_DEBUG, "[JNI] getRemoteSigners()");
    try {
        auto signers = NunchukProvider::get()->nu->GetRemoteSigners();
        return Deserializer::convert2JSigners(env, signers);
    } catch (BaseException &e) {
        syslog(LOG_DEBUG, "[JNI] getRemoteSigners error::%s", e.what());
        Deserializer::convert2JException(env, e);
        return env->ExceptionOccurred();
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return env->ExceptionOccurred();
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_deleteRemoteSigner(
        JNIEnv *env,
        jobject thiz,
        jstring master_fingerprint,
        jstring derivation_path
) {
    try {
        NunchukProvider::get()->nu->DeleteRemoteSigner(
                env->GetStringUTFChars(master_fingerprint, JNI_FALSE),
                env->GetStringUTFChars(derivation_path, JNI_FALSE
                ));
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        env->ExceptionOccurred();
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        env->ExceptionOccurred();
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_updateRemoteSigner(
        JNIEnv *env,
        jobject thiz,
        jobject signer
) {
    try {
        auto singleSigner = Serializer::convert2CSigner(env, signer);
        NunchukProvider::get()->nu->UpdateRemoteSigner(singleSigner);
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        env->ExceptionOccurred();
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        env->ExceptionOccurred();
    }
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_updateMasterSigner(
        JNIEnv *env,
        jobject thiz,
        jobject signer
) {
    try {
        auto masterSigner = Serializer::convert2CMasterSigner(env, signer);
        return NunchukProvider::get()->nu->UpdateMasterSigner(masterSigner);
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        env->ExceptionOccurred();
        return JNI_FALSE;
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return JNI_FALSE;
    }
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_generateMnemonic(
        JNIEnv *env,
        jobject thiz
) {
    try {
        return env->NewStringUTF(Utils::GenerateMnemonic().c_str());
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        env->ExceptionOccurred();
        return env->NewStringUTF("");
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        env->ExceptionOccurred();
        return env->NewStringUTF("");
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return env->NewStringUTF("");
    }
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_getBip39WordList(
        JNIEnv *env,
        jobject thiz
) {
    try {
        return Deserializer::convert2JListString(env, Utils::GetBIP39WordList());
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return env->ExceptionOccurred();
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return env->ExceptionOccurred();
    }
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_checkMnemonic(
        JNIEnv *env,
        jobject thiz,
        jstring mnemonic
) {
    try {
        return Utils::CheckMnemonic(env->GetStringUTFChars(mnemonic, JNI_FALSE));
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        env->ExceptionOccurred();
        return JNI_FALSE;
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        env->ExceptionOccurred();
        return JNI_FALSE;
    }
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_createSoftwareSigner(
        JNIEnv *env,
        jobject thiz,
        jstring name,
        jstring mnemonic,
        jstring passphrase, jboolean is_primary) {
    try {
        const MasterSigner &signer = NunchukProvider::get()->nu->CreateSoftwareSigner(
                env->GetStringUTFChars(name, JNI_FALSE),
                env->GetStringUTFChars(mnemonic, JNI_FALSE),
                env->GetStringUTFChars(passphrase, JNI_FALSE),
                [](int percent) { return true; },
                is_primary
        );
        return Deserializer::convert2JMasterSigner(env, signer);
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return env->ExceptionOccurred();
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return env->ExceptionOccurred();
    }
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_getMasterSigners(
        JNIEnv *env,
        jobject thiz
) {
    try {
        auto signers = NunchukProvider::get()->nu->GetMasterSigners();
        return Deserializer::convert2JMasterSigners(env, signers);
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return env->ExceptionOccurred();
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return env->ExceptionOccurred();
    }
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_getMasterSigner(
        JNIEnv *env,
        jobject thiz,
        jstring mastersigner_id
) {
    try {
        auto signer = NunchukProvider::get()->nu->GetMasterSigner(
                env->GetStringUTFChars(mastersigner_id, JNI_FALSE));
        return Deserializer::convert2JMasterSigner(env, signer);
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return env->ExceptionOccurred();
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return env->ExceptionOccurred();
    }
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_getSignersFromMasterSigner(
        JNIEnv *env,
        jobject thiz,
        jstring mastersigner_id
) {
    try {
        auto signer = NunchukProvider::get()->nu->GetSignersFromMasterSigner(
                env->GetStringUTFChars(mastersigner_id, JNI_FALSE));
        return Deserializer::convert2JSigners(env, signer);
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return env->ExceptionOccurred();
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return env->ExceptionOccurred();
    }
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_getUnusedSignerFromMasterSigner(
        JNIEnv *env,
        jobject thiz,
        jstring mastersigner_id,
        jint wallet_type,
        jint address_type
) {
    try {
        auto signer = NunchukProvider::get()->nu->GetUnusedSignerFromMasterSigner(
                env->GetStringUTFChars(mastersigner_id, JNI_FALSE),
                Serializer::convert2CWalletType(wallet_type),
                Serializer::convert2CAddressType(address_type)
        );
        return Deserializer::convert2JSigner(env, signer);
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return env->ExceptionOccurred();
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return env->ExceptionOccurred();
    }
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_deleteMasterSigner(
        JNIEnv *env,
        jobject thiz,
        jstring mastersigner_id
) {
    try {
        return NunchukProvider::get()->nu->DeleteMasterSigner(
                env->GetStringUTFChars(mastersigner_id, JNI_FALSE));
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        env->ExceptionOccurred();
        return JNI_FALSE;
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        env->ExceptionOccurred();
        return JNI_FALSE;
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_sendSignerPassphrase(
        JNIEnv *env,
        jobject thiz,
        jstring mastersigner_id,
        jstring passphrase
) {
    try {
        NunchukProvider::get()->nu->SendSignerPassphrase(
                env->GetStringUTFChars(mastersigner_id, JNI_FALSE),
                env->GetStringUTFChars(passphrase, JNI_FALSE)
        );
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        env->ExceptionOccurred();
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        env->ExceptionOccurred();
    }
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_healthCheckMasterSigner(
        JNIEnv *env,
        jobject thiz,
        jstring fingerprint,
        jstring message,
        jstring signature,
        jstring path
) {
    syslog(LOG_DEBUG, "[JNI] healthCheckMasterSigner()");
    try {
        std::string _message = (env)->GetStringUTFChars(message, JNI_FALSE);
        std::string _signature = (env)->GetStringUTFChars(signature, JNI_FALSE);
        std::string _path = (env)->GetStringUTFChars(path, JNI_FALSE);
        auto healthStatus = NunchukProvider::get()->nu->HealthCheckMasterSigner(
                env->GetStringUTFChars(fingerprint, JNI_FALSE),
                _message,
                _signature,
                _path
        );
        return Deserializer::convert2JHealthStatus(env, healthStatus);
    } catch (BaseException &e) {
        syslog(LOG_DEBUG, "[JNI] healthCheckMasterSigner error::%s", e.what());
        Deserializer::convert2JException(env, e);
        return env->ExceptionOccurred();
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return env->ExceptionOccurred();
    }
}
extern "C"
JNIEXPORT void JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_clearSignerPassphrase(JNIEnv *env,
                                                                           jobject thiz,
                                                                           jstring master_signer_id) {
    try {
        NunchukProvider::get()->nu->ClearSignerPassphrase(
                env->GetStringUTFChars(master_signer_id, JNI_FALSE));
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        env->ExceptionOccurred();
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        env->ExceptionOccurred();
    }
}
extern "C"
JNIEXPORT jstring JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_getPrimaryKeyAddress(JNIEnv *env, jobject thiz,
                                                                          jstring mnemonic,
                                                                          jstring passphrase) {
    try {
        return env->NewStringUTF(
                Utils::GetPrimaryKeyAddress(StringWrapper(env, mnemonic),
                                            StringWrapper(env, passphrase)).c_str());
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
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_signLoginMessage(JNIEnv *env, jobject thiz,
                                                                      jstring mnemonic,
                                                                      jstring passphrase,
                                                                      jstring message) {
    try {
        return env->NewStringUTF(
                Utils::SignLoginMessage(StringWrapper(env, mnemonic),
                                        StringWrapper(env, passphrase),
                                        StringWrapper(env, message)).c_str());
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
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_getMasterFingerprint(JNIEnv *env, jobject thiz,
                                                                          jstring mnemonic,
                                                                          jstring passphrase) {
    try {
        return env->NewStringUTF(
                Utils::GetMasterFingerprint(StringWrapper(env, mnemonic),
                                            StringWrapper(env, passphrase)).c_str());
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
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_getPrimaryKeys(JNIEnv *env, jobject thiz,
                                                                    jint chain,
                                                                    jstring storage_path) {
    try {
        auto keys = Utils::GetPrimaryKeys(StringWrapper(env, storage_path),
                                          Serializer::convert2CChain(chain));
        return Deserializer::convert2JPrimaryKeys(env, keys);
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return env->ExceptionOccurred();
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return env->ExceptionOccurred();
    }
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_signLoginMessageImpl(JNIEnv *env, jobject thiz,
                                                                          jstring mastersigner_id,
                                                                          jstring message) {
    try {
        return env->NewStringUTF(
                NunchukProvider::get()->nu->SignLoginMessage(StringWrapper(env, mastersigner_id),
                                                             StringWrapper(env, message)).c_str());
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
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_deletePrimaryKey(JNIEnv *env, jobject thiz) {
    try {
        return NunchukProvider::get()->nu->DeletePrimaryKey();
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return JNI_FALSE;
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return JNI_FALSE;
    }
}
extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_getSignerFromMasterSigner(JNIEnv *env,
                                                                               jobject thiz,
                                                                               jstring master_signer_id,
                                                                               jstring path) {
    try {
        auto signer = NunchukProvider::get()->nu->GetSignerFromMasterSigner(
                StringWrapper(env, master_signer_id),
                StringWrapper(env, path)
        );
        return Deserializer::convert2JSigner(env, signer);
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return JNI_FALSE;
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return JNI_FALSE;
    }
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_getSignerFromMasterSignerByIndex(JNIEnv *env,
                                                                                      jobject thiz,
                                                                                      jstring master_signer_id,
                                                                                      jint wallet_type,
                                                                                      jint address_type,
                                                                                      jint index) {
    try {
        auto signer = NunchukProvider::get()->nu->GetSignerFromMasterSigner(
                StringWrapper(env, master_signer_id),
                Serializer::convert2CWalletType(wallet_type),
                Serializer::convert2CAddressType(address_type),
                index
        );
        return Deserializer::convert2JSigner(env, signer);
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return JNI_FALSE;
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return JNI_FALSE;
    }
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_getDefaultSignerFromMasterSigner(JNIEnv *env,
                                                                                      jobject thiz,
                                                                                      jstring master_signer_id,
                                                                                      jint wallet_type,
                                                                                      jint address_type) {
    try {
        auto signer = NunchukProvider::get()->nu->GetDefaultSignerFromMasterSigner(
                StringWrapper(env, master_signer_id),
                Serializer::convert2CWalletType(wallet_type),
                Serializer::convert2CAddressType(address_type)
        );
        return Deserializer::convert2JSigner(env, signer);
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return env->ExceptionOccurred();
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return env->ExceptionOccurred();
    }
}
extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_parseJsonSigners(JNIEnv *env, jobject thiz,
                                                                      jstring str, jobject type) {
    try {
        auto signers = NunchukProvider::get()->nu->ParseJSONSigners(
                StringWrapper(env, str),
                Serializer::convert2CSignerType(env, type)
        );
        return Deserializer::convert2JSigners(env, signers);
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return JNI_FALSE;
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return JNI_FALSE;
    }
}
extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_signerTypeFromStr(JNIEnv *env, jobject thiz,
                                                                       jstring signer_type) {
    try {
        auto signerType = SignerTypeFromStr(
                StringWrapper(env, signer_type)
        );
        return Deserializer::convert2JSignerType(env, signerType);
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return Deserializer::convert2JSignerType(env, SignerType::UNKNOWN);
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return Deserializer::convert2JSignerType(env, SignerType::UNKNOWN);
    }
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_signHealthCheckMessageTapSigner(JNIEnv *env,
                                                                                     jobject thiz,
                                                                                     jobject iso_dep,
                                                                                     jstring cvc,
                                                                                     jobject signer,
                                                                                     jstring messages_to_sign) {
    try {
        auto ts = NunchukProvider::get()->nu->CreateTapsigner(NFC::makeTransport(env, iso_dep));
        auto singleSigner = Serializer::convert2CSigner(env, signer);
        std::string signature = NunchukProvider::get()->nu->SignHealthCheckMessage(ts.get(),
                                                                                   StringWrapper(env, cvc),
                                                                                   singleSigner,
                                                                                   StringWrapper(env,messages_to_sign));
        return env->NewStringUTF(signature.c_str());
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
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_signHealthCheckMessage(JNIEnv *env,
                                                                            jobject thiz,
                                                                            jobject signer,
                                                                            jstring messages_to_sign) {
    try {
        auto singleSigner = Serializer::convert2CSigner(env, signer);
        std::string signature = NunchukProvider::get()->nu->SignHealthCheckMessage(singleSigner,
                                                                                   StringWrapper(
                                                                                           env,
                                                                                           messages_to_sign));
        return env->NewStringUTF(signature.c_str());
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
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_signMessageBySoftwareKey(JNIEnv *env,
                                                                              jobject thiz,
                                                                              jstring message,
                                                                              jstring path,
                                                                              jstring master_signer_id) {
    try {
        SingleSigner signer = NunchukProvider::get()->nu->GetSignerFromMasterSigner(
                StringWrapper(env, master_signer_id),
                StringWrapper(env, path)
                );
        std::string signature = NunchukProvider::get()->nu->SignMessage(signer, StringWrapper(env, message));
        std::string address = NunchukProvider::get()->nu->GetSignerAddress(signer);
        std::string rfc2440 = ExportBitcoinSignedMessage(
                BitcoinSignedMessage{StringWrapper(env, message), address, signature});
        return Deserializer::convert2JSignedMessage(env, address, signature, rfc2440);
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
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_getHealthCheckPath(JNIEnv *env, jobject thiz) {
    try {
        std::string default_path = NunchukProvider::get()->nu->GetHealthCheckPath();
        return env->NewStringUTF(default_path.c_str());
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return nullptr;
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return nullptr;
    }
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_getCurrentSignerIndex(JNIEnv *env,
                                                                           jobject thiz,
                                                                           jstring xfp,
                                                                           jint wallet_type,
                                                                           jint address_type) {
    try {
        return NunchukProvider::get()->nu->GetLastUsedSignerIndex(
                StringWrapper(env, xfp),
                Serializer::convert2CWalletType(wallet_type),
                Serializer::convert2CAddressType(address_type)
        );
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return -1;
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return -1;
    }
}
extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_getSignerByIndex(JNIEnv *env,
                                                                      jobject thiz,
                                                                      jstring xfp,
                                                                      jint wallet_type,
                                                                      jint address_type,
                                                                      jint index) {
    try {
        auto signer = NunchukProvider::get()->nu->GetSigner(
                StringWrapper(env, xfp),
                Serializer::convert2CWalletType(wallet_type),
                Serializer::convert2CAddressType(address_type),
                index
        );
        return Deserializer::convert2JSigner(env, signer);
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return nullptr;
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return nullptr;
    }
}