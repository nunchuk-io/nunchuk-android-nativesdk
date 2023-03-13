#include <cstring>
#include <jni.h>
#include <nunchuk.h>
#include "nunchukprovider.h"
#include "serializer.h"
#include "deserializer.h"
#include "string-wrapper.h"

using namespace nunchuk;

extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_getWallets(JNIEnv *env, jobject thiz) {
    try {
        auto wallets = NunchukProvider::get()->nu->GetWallets();
        return Deserializer::convert2JWallets(env, wallets);
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
    } catch (BaseException &e) {
        syslog(LOG_DEBUG, "[JNI] draftWallet error::%s", e.what());
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
    } catch (BaseException &e) {
        syslog(LOG_DEBUG, "[JNI] createWallet error::%s", e.what());
        Deserializer::convert2JException(env, e);
        return env->ExceptionOccurred();
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
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
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_exportCoboWallet(JNIEnv *env, jobject thiz,
                                                                      jstring wallet_id) {
    try {
        auto values = NunchukProvider::get()->nu->ExportCoboWallet(
                env->GetStringUTFChars(wallet_id, JNI_FALSE)
        );
        return Deserializer::convert2JListString(env, values);
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
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_exportKeystoneWallet(
        JNIEnv *env,
        jobject thiz,
        jstring wallet_id,
        jint density
) {
    try {
        auto values = NunchukProvider::get()->nu->ExportKeystoneWallet(
                env->GetStringUTFChars(wallet_id, JNI_FALSE),
                density
        );
        return Deserializer::convert2JListString(env, values);
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
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_importKeystoneWallet(
        JNIEnv *env,
        jobject thiz,
        jobject qr_data,
        jstring description
) {
    auto cQrData = Serializer::convert2CListString(env, qr_data);
    try {
        auto wallet = NunchukProvider::get()->nu->ImportKeystoneWallet(
                cQrData,
                StringWrapper(env, description)
        );
        return Deserializer::convert2JWallet(env, wallet);
    } catch (BaseException &e) {
        try {
            if (cQrData.size() != 1) {
                Deserializer::convert2JException(env, e);
                return JNI_FALSE;
            }
            auto wallet = Utils::ParseWalletDescriptor(cQrData[0]);
            wallet.set_create_date(std::time(nullptr));
            NunchukProvider::get()->nu->CreateWallet(wallet, true);
            return Deserializer::convert2JWallet(env, wallet);
        } catch(BaseException &e) {
            Deserializer::convert2JException(env, e);
            return JNI_FALSE;
        }
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return JNI_FALSE;
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
        const auto &wallet = NunchukProvider::get()->nu->GetWallet(
                env->GetStringUTFChars(wallet_id, JNI_FALSE));
        return Deserializer::convert2JWallet(env, wallet);
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
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_updateWallet(
        JNIEnv *env,
        jobject thiz,
        jobject wallet
) {
    try {
        auto updateWallet = Serializer::convert2CWallet(env, wallet);
        return NunchukProvider::get()->nu->UpdateWallet(updateWallet);
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
JNIEXPORT jboolean JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_deleteWallet(
        JNIEnv *env,
        jobject thiz,
        jstring wallet_id
) {
    try {
        return NunchukProvider::get()->nu->DeleteWallet(
                env->GetStringUTFChars(wallet_id, JNI_FALSE));
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
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_importWallet(
        JNIEnv *env,
        jobject thiz,
        jstring file_path,
        jstring name,
        jstring description
) {
    try {
        auto wallet = NunchukProvider::get()->nu->ImportWalletDescriptor(
                env->GetStringUTFChars(file_path, JNI_FALSE),
                env->GetStringUTFChars(name, JNI_FALSE),
                env->GetStringUTFChars(description, JNI_FALSE)
        );
        return Deserializer::convert2JWallet(env, wallet);
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
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_parseWalletDescriptor(
        JNIEnv *env,
        jobject thiz,
        jstring content
) {
    try {
        auto wallet = NunchukProvider::get()->nuUtils->ParseWalletDescriptor(
                env->GetStringUTFChars(content, JNI_FALSE)
        );
        return Deserializer::convert2JWallet(env, wallet);
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
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_hasSigner(
        JNIEnv *env,
        jobject thiz,
        jobject signer
) {
    try {
        return NunchukProvider::get()->nu->HasSigner(Serializer::convert2CSigner(env, signer));
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
JNIEXPORT jboolean JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_setSelectedWallet(
        JNIEnv *env,
        jobject thiz,
        jstring wallet_id
) {
    try {
        return NunchukProvider::get()->nu->SetSelectedWallet(StringWrapper(env, wallet_id));
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
JNIEXPORT jstring JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_exportWalletToBsmsById(JNIEnv *env, jobject thiz,
                                                                        jstring wallet_id) {
    try {
        auto data = NunchukProvider::get()->nu->GetWalletExportData(StringWrapper(env, wallet_id),
                                                                    ExportFormat::BSMS);
        return env->NewStringUTF(data.c_str());
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return JNI_FALSE;
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return JNI_FALSE;
    }
}
extern "C"
JNIEXPORT jboolean JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_hasWallet(JNIEnv *env, jobject thiz,
                                                               jstring wallet_id) {
    try {
        return NunchukProvider::get()->nu->HasWallet(StringWrapper(env, wallet_id));
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
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_exportWalletToBsms(JNIEnv *env, jobject thiz,
                                                                        jobject wallet) {
    try {
        auto data = NunchukProvider::get()->nu->GetWalletExportData(Serializer::convert2CWallet(env, wallet),
                                                                    ExportFormat::BSMS);
        return env->NewStringUTF(data.c_str());
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return JNI_FALSE;
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return JNI_FALSE;
    }
}
extern "C"
JNIEXPORT void JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_forceRefreshWallet(JNIEnv *env, jobject thiz,
                                                                        jstring wallet_id) {
    try {
        NunchukProvider::get()->nu->ForceRefreshWallet(StringWrapper(env, wallet_id));
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
    }
}