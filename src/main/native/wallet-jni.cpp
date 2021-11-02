#include <cstring>
#include <jni.h>
#include <nunchuk.h>
#include "nunchukprovider.h"
#include "serializer.h"
#include "deserializer.h"

using namespace nunchuk;

extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_getWallets(JNIEnv *env, jobject thiz) {
    try {
        auto wallets = NunchukProvider::get()->nu->GetWallets();
        return Deserializer::convert2JWallets(env, wallets);
    } catch (std::exception &e) {
        Deserializer::convert2JException(env, e.what());
        return env->ExceptionOccurred();
    }
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_draftWallet(
        JNIEnv *env,
        jobject thiz,
        jstring name,
        jint require_signs,
        jobject signers,
        jint address_type,
        jboolean is_escrow,
        jstring description
) {
    syslog(LOG_DEBUG, "[JNI] draftWallet()");
    syslog(LOG_DEBUG, "[JNI] name::%s", env->GetStringUTFChars(name, JNI_FALSE));
    syslog(LOG_DEBUG, "[JNI] require_signs::%d", require_signs);
    try {
        const std::vector<SingleSigner> &singleSigners = Serializer::convert2CSigners(env, signers);
        AddressType type = Serializer::convert2CAddressType(address_type);
        auto filePath = NunchukProvider::get()->nu->DraftWallet(
                env->GetStringUTFChars(name, JNI_FALSE),
                require_signs,
                singleSigners.size(),
                singleSigners,
                type,
                is_escrow,
                env->GetStringUTFChars(description, JNI_FALSE)
        );
        return env->NewStringUTF(filePath.c_str());
    } catch (std::exception &e) {
        syslog(LOG_DEBUG, "[JNI] draftWallet error::%s", e.what());
        Deserializer::convert2JException(env, e.what());
        env->ExceptionOccurred();
        return env->NewStringUTF("");
    }
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_createWallet(
        JNIEnv *env,
        jobject thiz,
        jstring name,
        jint require_signs,
        jobject signers,
        jint address_type,
        jboolean is_escrow,
        jstring description
) {
    syslog(LOG_DEBUG, "[JNI] createWallet()");
    syslog(LOG_DEBUG, "[JNI] name::%s", env->GetStringUTFChars(name, JNI_FALSE));
    syslog(LOG_DEBUG, "[JNI] require_signs::%d", require_signs);
    try {
        const std::vector<SingleSigner> &singleSigners = Serializer::convert2CSigners(env, signers);
        AddressType type = Serializer::convert2CAddressType(address_type);
        const Wallet &wallet = NunchukProvider::get()->nu->CreateWallet(
                env->GetStringUTFChars(name, JNI_FALSE),
                require_signs,
                singleSigners.size(),
                singleSigners,
                type,
                is_escrow,
                env->GetStringUTFChars(description, JNI_FALSE)
        );
        return Deserializer::convert2JWallet(env, wallet);
    } catch (std::exception &e) {
        syslog(LOG_DEBUG, "[JNI] createWallet error::%s", e.what());
        Deserializer::convert2JException(env, e.what());
        return env->ExceptionOccurred();
    }
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_exportWallet(
        JNIEnv *env,
        jobject thiz,
        jstring wallet_id,
        jstring file_path,
        jint format
) {
    try {
        return NunchukProvider::get()->nu->ExportWallet(
                env->GetStringUTFChars(wallet_id, JNI_FALSE),
                env->GetStringUTFChars(file_path, JNI_FALSE),
                Serializer::convert2CExportFormat(format)
        );
    } catch (std::exception &e) {
        Deserializer::convert2JException(env, e.what());
        env->ExceptionOccurred();
        return JNI_FALSE;
    }
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_exportCoboWallet(JNIEnv *env, jobject thiz, jstring wallet_id) {
    try {
        auto values =  NunchukProvider::get()->nu->ExportCoboWallet(
            env->GetStringUTFChars(wallet_id, JNI_FALSE)
        );
        return Deserializer::convert2JListString(env, values);
    } catch (std::exception &e) {
        Deserializer::convert2JException(env, e.what());
        return env->ExceptionOccurred();
    }
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_exportKeystoneWallet(
        JNIEnv *env,
        jobject thiz,
        jstring wallet_id
) {
    try {
        auto values =  NunchukProvider::get()->nu->ExportKeystoneWallet(
                env->GetStringUTFChars(wallet_id, JNI_FALSE)
        );
        return Deserializer::convert2JListString(env, values);
    } catch (std::exception &e) {
        Deserializer::convert2JException(env, e.what());
        return env->ExceptionOccurred();
    }
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_importKeystoneWallet(
        JNIEnv *env,
        jobject thiz,
        jobject qr_data,
        jstring description
) {
    try {
        auto values =  NunchukProvider::get()->nu->ImportKeystoneWallet(
                Serializer::convert2CListString(env, qr_data),
                env->GetStringUTFChars(description, JNI_FALSE)
        );
        return Deserializer::convert2JWallet(env, values);
    } catch (std::exception &e) {
        Deserializer::convert2JException(env, e.what());
        return env->ExceptionOccurred();
    }
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_getWallet(
        JNIEnv *env,
        jobject thiz,
        jstring wallet_id
) {
    try {
        const auto &wallet = NunchukProvider::get()->nu->GetWallet(env->GetStringUTFChars(wallet_id, JNI_FALSE));
        return Deserializer::convert2JWallet(env, wallet);
    } catch (std::exception &e) {
        Deserializer::convert2JException(env, e.what());
        return env->ExceptionOccurred();
    }
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_updateWallet(
        JNIEnv *env,
        jobject thiz,
        jobject wallet
) {
    try {
        auto updateWallet = Serializer::convert2CWallet(env, wallet);
        return NunchukProvider::get()->nu->UpdateWallet(updateWallet);
    }   catch (std::exception &e) {
        Deserializer::convert2JException(env, e.what());
        env->ExceptionOccurred();
        return JNI_FALSE;
    }
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_deleteWallet(
        JNIEnv *env,
        jobject thiz,
        jstring wallet_id
) {
    try {
        return NunchukProvider::get()->nu->DeleteWallet(env->GetStringUTFChars(wallet_id, JNI_FALSE));
    }   catch (std::exception &e) {
        Deserializer::convert2JException(env, e.what());
        env->ExceptionOccurred();
        return JNI_FALSE;
    }
}

