#include <cstring>
#include <jni.h>
#include <nunchuk.h>
#include "nunchukprovider.h"
#include "serializer.h"
#include "deserializer.h"

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
        jstring master_fingerprint
) {
    try {
        const SingleSigner &signer = NunchukProvider::get()->nu->CreateSigner(
                env->GetStringUTFChars(name, JNI_FALSE),
                env->GetStringUTFChars(xpub, JNI_FALSE),
                env->GetStringUTFChars(public_key, JNI_FALSE),
                env->GetStringUTFChars(derivation_path, JNI_FALSE),
                env->GetStringUTFChars(master_fingerprint, JNI_FALSE)
        );
        return Deserializer::convert2JSigner(env, signer);
    } catch (std::exception &e) {
        Deserializer::convert2JException(env, e.what());
        return env->ExceptionOccurred();
    }
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_createCoboSigner(
        JNIEnv *env,
        jobject thiz,
        jstring name,
        jstring jsonInfo
) {
    try {
        const SingleSigner &signer = NunchukProvider::get()->nu->CreateCoboSigner(
                env->GetStringUTFChars(name, JNI_FALSE),
                env->GetStringUTFChars(jsonInfo, JNI_FALSE)
        );
        return Deserializer::convert2JSigner(env, signer);
    } catch (std::exception &e) {
        Deserializer::convert2JException(env, e.what());
        return env->ExceptionOccurred();
    }
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_createKeystoneSigner(
        JNIEnv *env,
        jobject thiz,
        jstring name,
        jstring qr_data
) {
    try {
        const SingleSigner &signer = NunchukProvider::get()->nu->CreateKeystoneSigner(
                env->GetStringUTFChars(name, JNI_FALSE),
                env->GetStringUTFChars(qr_data, JNI_FALSE)
        );
        return Deserializer::convert2JSigner(env, signer);
    } catch (std::exception &e) {
        Deserializer::convert2JException(env, e.what());
        return env->ExceptionOccurred();
    }
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_getRemoteSigners(JNIEnv *env, jobject thiz) {
    try {
        auto signers = NunchukProvider::get()->nu->GetRemoteSigners();
        return Deserializer::convert2JSigners(env, signers);
    } catch (std::exception &e) {
        Deserializer::convert2JException(env, e.what());
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
    } catch (std::exception &e) {
        Deserializer::convert2JException(env, e.what());
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
    } catch (std::exception &e) {
        Deserializer::convert2JException(env, e.what());
        env->ExceptionOccurred();
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_updateMasterSigner(
        JNIEnv *env,
        jobject thiz,
        jobject signer
) {
    try {
        auto masterSigner = Serializer::convert2CMasterSigner(env, signer);
        NunchukProvider::get()->nu->UpdateMasterSigner(masterSigner);
    } catch (std::exception &e) {
        Deserializer::convert2JException(env, e.what());
        env->ExceptionOccurred();
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
    } catch (std::exception &e) {
        Deserializer::convert2JException(env, e.what());
        env->ExceptionOccurred();
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
    } catch (std::exception &e) {
        Deserializer::convert2JException(env, e.what());
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
    } catch (std::exception &e) {
        Deserializer::convert2JException(env, e.what());
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
        jstring passphrase
) {
    try {
        const MasterSigner &signer = NunchukProvider::get()->nu->CreateSoftwareSigner(
                env->GetStringUTFChars(name, JNI_FALSE),
                env->GetStringUTFChars(mnemonic, JNI_FALSE),
                env->GetStringUTFChars(passphrase, JNI_FALSE),
                [](int percent) { return true; }
        );
        return Deserializer::convert2JMasterSigner(env, signer);
    } catch (std::exception &e) {
        Deserializer::convert2JException(env, e.what());
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
    } catch (std::exception &e) {
        Deserializer::convert2JException(env, e.what());
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
        auto signer = NunchukProvider::get()->nu->GetMasterSigner(env->GetStringUTFChars(mastersigner_id, JNI_FALSE));
        return Deserializer::convert2JMasterSigner(env, signer);
    } catch (std::exception &e) {
        Deserializer::convert2JException(env, e.what());
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
        auto signer = NunchukProvider::get()->nu->GetSignersFromMasterSigner(env->GetStringUTFChars(mastersigner_id, JNI_FALSE));
        return Deserializer::convert2JSigners(env, signer);
    } catch (std::exception &e) {
        Deserializer::convert2JException(env, e.what());
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
    } catch (std::exception &e) {
        Deserializer::convert2JException(env, e.what());
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
        return NunchukProvider::get()->nu->DeleteMasterSigner(env->GetStringUTFChars(mastersigner_id, JNI_FALSE));
    } catch (std::exception &e) {
        Deserializer::convert2JException(env, e.what());
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
    } catch (std::exception &e) {
        Deserializer::convert2JException(env, e.what());
        env->ExceptionOccurred();
    }
}