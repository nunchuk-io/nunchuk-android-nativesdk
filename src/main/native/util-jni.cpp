#include <cstring>
#include <jni.h>
#include <syslog.h>
#include <nunchuk.h>
#include "nunchukprovider.h"
#include "serializer.h"
#include "deserializer.h"
#include "initializer.h"
#include "string-wrapper.h"

using namespace nunchuk;

extern "C"
JNIEXPORT jstring JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_valueFromAmount(
        JNIEnv *env,
        jobject thiz,
        jobject amount
) {
    try {
        Amount _amount = Serializer::convert2CAmount(env, amount);
        auto value = Utils::ValueFromAmount(_amount);
        return env->NewStringUTF(value.c_str());
    } catch (BaseException &e) {
        syslog(LOG_DEBUG, "[JNI] valueFromAmount error::%s", e.what());
        Deserializer::convert2JException(env, e);
        env->ExceptionOccurred();
        return env->NewStringUTF("");
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        env->ExceptionOccurred();
        return env->NewStringUTF("");
    }
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_isValidAddress(
        JNIEnv *env,
        jobject thiz,
        jstring address
) {
    try {
        Utils::AddressToScriptPubKey(env->GetStringUTFChars(address, JNI_FALSE));
        return JNI_TRUE;
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
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_getDevices(
        JNIEnv *env,
        jobject thiz
) {
    auto devices = NunchukProvider::get()->nu->GetDevices();
    return Deserializer::convert2JDevices(env, devices);
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_getChainTip(
        JNIEnv *env,
        jobject thiz
) {
    try {
        return NunchukProvider::get()->nu->GetChainTip();
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        env->ExceptionOccurred();
        return -1;
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        env->ExceptionOccurred();
        return -1;
    }
}


extern "C"
JNIEXPORT jboolean JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_isValidDerivationPath(JNIEnv *env,
                                                                           jobject thiz,
                                                                           jstring path) {
    try {
        return Utils::IsValidDerivationPath(StringWrapper(env, path));
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return JNI_FALSE;
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return JNI_FALSE;
    }
}
extern "C"
JNIEXPORT jstring JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_createRequestToken(JNIEnv *env, jobject thiz,
                                                                        jstring signature,
                                                                        jstring fingerprint) {
    try {
        auto token = Utils::CreateRequestToken(StringWrapper(env, signature),
                                               StringWrapper(env, fingerprint));
        return env->NewStringUTF(token.c_str());
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
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_getHealthCheckMessage(JNIEnv *env,
                                                                           jobject thiz,
                                                                           jstring body) {
    try {
        auto messages_to_sign = Utils::GetHealthCheckMessage(StringWrapper(env, body));
        return env->NewStringUTF(messages_to_sign.c_str());
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
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_getHealthCheckDummyTxMessage(JNIEnv *env,
                                                                                  jobject thiz,
                                                                                  jstring wallet_id,
                                                                                  jstring body) {
    try {
        auto wallet = NunchukProvider::get()->nu->GetWallet(
                StringWrapper(env, wallet_id)
        );
        std::string tx_to_sign = Utils::GetHealthCheckDummyTx(wallet, StringWrapper(env,
                                                                                    body)); // user_data in json string
        return env->NewStringUTF(tx_to_sign.c_str());
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
    }
}
extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_getDummyTx(JNIEnv *env, jobject thiz,
                                                                jstring wallet_id,
                                                                jstring message) {
    try {
        auto wallet = NunchukProvider::get()->nu->GetWallet(
                StringWrapper(env, wallet_id)
        );
        auto tx = Utils::DecodeDummyTx(wallet, StringWrapper(env, message));
        return Deserializer::convert2JTransaction(env, tx);
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
    }
    return nullptr;
}
extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_exportKeystoneDummyTransaction(JNIEnv *env,
                                                                                    jobject thiz,
                                                                                    jstring tx_to_sign,
                                                                                    jint density) {
    try {
        auto qrs = Utils::ExportKeystoneTransaction(StringWrapper(env, tx_to_sign), density);
        return Deserializer::convert2JListString(env, qrs);
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
    }
    return nullptr;
}
extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_exportPassportDummyTransaction(JNIEnv *env,
                                                                                    jobject thiz,
                                                                                    jstring tx_to_sign,
                                                                                    jint density) {
    try {
        auto qrs = Utils::ExportPassportTransaction(StringWrapper(env, tx_to_sign), density);
        return Deserializer::convert2JListString(env, qrs);
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
    }
    return nullptr;
}
extern "C"
JNIEXPORT jstring JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_parseKeystoneDummyTransaction(JNIEnv *env,
                                                                                   jobject thiz,
                                                                                   jobject qrs) {
    try {
        std::string psbt = Utils::ParseKeystoneTransaction(
                Serializer::convert2CListString(env, qrs));
        return env->NewStringUTF(psbt.c_str());
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
    }
    return nullptr;
}
extern "C"
JNIEXPORT jstring JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_parsePassportDummyTransaction(JNIEnv *env,
                                                                                   jobject thiz,
                                                                                   jobject qrs) {
    try {
        std::string psbt = Utils::ParsePassportTransaction(
                Serializer::convert2CListString(env, qrs));
        return env->NewStringUTF(psbt.c_str());
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
    }
    return nullptr;
}
extern "C"
JNIEXPORT jstring JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_getDummyTransactionSignature(JNIEnv *env,
                                                                                  jobject thiz,
                                                                                  jobject signer,
                                                                                  jstring psbt) {
    try {
        auto singleSigner = Serializer::convert2CSigner(env, signer);
        auto signature = Utils::GetPartialSignature(singleSigner, StringWrapper(env, psbt));
        return env->NewStringUTF(signature.c_str());
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
    }
    return nullptr;
}
extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_getDummyTxByByteArray(JNIEnv *env,
                                                                           jobject thiz,
                                                                           jstring wallet_id,
                                                                           jbyteArray file_data) {
    try {
        auto first = env->GetByteArrayElements(file_data, nullptr);
        auto len = env->GetArrayLength(file_data);
        std::string psbt = std::string(first, first + len);
        env->ReleaseByteArrayElements(file_data, first, JNI_ABORT);
        auto wallet = NunchukProvider::get()->nu->GetWallet(
                StringWrapper(env, wallet_id)
        );
        auto tx = Utils::DecodeDummyTx(wallet, psbt);
        return Deserializer::convert2JTransaction(env, tx);
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
    }
    return nullptr;
}
extern "C"
JNIEXPORT jdouble JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_analyzeQr(JNIEnv *env, jobject thiz,
                                                               jobject qrs) {
    try {
        return Utils::AnalyzeQR(Serializer::convert2CListString(env, qrs)).estimated_percent_complete;
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return 0.0;
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return 0.0;
    }
}
extern "C"
JNIEXPORT jstring JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_hashSHA256(JNIEnv *env, jobject thiz,
                                                                jstring data) {
    try {
        std::string hashedData = Utils::SHA256(StringWrapper(env, data));
        return env->NewStringUTF(hashedData.c_str());
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
    }
}