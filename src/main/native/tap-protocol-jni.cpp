#include <jni.h>
#include <string>
#include <tap_protocol/tap_protocol.h>
#include <tap_protocol/cktapcard.h>
#include <iostream>
#include <android/log.h>
#include <syslog.h>
#include "deserializer.h"
#include "nunchukprovider.h"

#define APPNAME "tap_protocol_native_sdk"

std::unique_ptr<tap_protocol::Transport> makeTransport(JNIEnv *env, jobject iso_dep) {
    static jclass isoDepCallerClass = NULL;
    static jmethodID transceiveMethodID = NULL;
    static jclass throwableClass = NULL;
    static jmethodID toStringMethodID = NULL;

    if (isoDepCallerClass == NULL) {
        isoDepCallerClass = static_cast<jclass>(env->NewGlobalRef(
                env->FindClass("com/nunchuk/android/nfc/IsoDepCaller")));
        transceiveMethodID = env->GetStaticMethodID(
                isoDepCallerClass, "transceive",
                "(Landroid/nfc/tech/IsoDep;[B)[B");
        throwableClass = static_cast<jclass>(env->NewGlobalRef(
                env->FindClass("java/lang/Throwable")));
        toStringMethodID = env->GetMethodID(throwableClass, "toString", "()Ljava/lang/String;");
    }

    auto tp = tap_protocol::MakeDefaultTransport([=](const tap_protocol::Bytes &in) {
        auto bytesToSend = env->NewByteArray(in.size());
        env->SetByteArrayRegion(bytesToSend, 0, in.size(), (jbyte *) in.data());

        auto bytesReceive = static_cast<jbyteArray> (env->CallStaticObjectMethod(isoDepCallerClass,
                                                                                 transceiveMethodID,
                                                                                 iso_dep,
                                                                                 bytesToSend));

        env->DeleteLocalRef(bytesToSend);
        if (jthrowable ex = env->ExceptionOccurred(); ex != NULL) {
            env->ExceptionClear();
            jstring exMessage = static_cast<jstring>(env->CallObjectMethod(ex,
                                                                           toStringMethodID));
            const char *exMessageChar = env->GetStringUTFChars(exMessage, JNI_FALSE);
            std::string exMessageCpp(exMessageChar);

            env->ReleaseStringUTFChars(exMessage, exMessageChar);
            env->DeleteLocalRef(exMessage);
            throw nunchuk::TapProtocolException(nunchuk::TapProtocolException::TAP_PROTOCOL_ERROR,
                                                exMessageCpp);
        }

        auto firstByte = env->GetByteArrayElements(bytesReceive, JNI_FALSE);
        tap_protocol::Bytes result((char *) firstByte,
                                   (char *) firstByte + env->GetArrayLength(bytesReceive));
        env->ReleaseByteArrayElements(bytesReceive, firstByte, JNI_ABORT);
        env->DeleteLocalRef(bytesReceive);
        return result;
    });
    return tp;
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_tapSignerStatus(JNIEnv *env, jobject thiz,
                                                                     jobject iso_dep) {
    try {
        auto ts = NunchukProvider::get()->nu->CreateTapsigner(makeTransport(env, iso_dep));
        auto status = NunchukProvider::get()->nu->GetTapsignerStatus(ts.get());
        return Deserializer::convert2JTapSignerStatus(env, status);
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
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_setupTapSigner(JNIEnv *env, jobject thiz,
                                                                    jobject iso_dep,
                                                                    jstring old_cvc,
                                                                    jstring new_cvc) {
    try {
        auto ts = NunchukProvider::get()->nu->CreateTapsigner(makeTransport(env, iso_dep));
        auto status = NunchukProvider::get()->nu->SetupTapsigner(
                ts.get(),
                env->GetStringUTFChars(old_cvc, JNI_FALSE),
                env->GetStringUTFChars(new_cvc, JNI_FALSE)
        );
        auto back_up_data = status.get_backup_data();
        back_up_data.push_back('\0');
        return env->NewStringUTF((char *) back_up_data.data());
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
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_createTapSigner(JNIEnv *env, jobject thiz,
                                                                     jobject iso_dep,
                                                                     jstring cvc,
                                                                     jstring name) {
    try {
        auto ts = NunchukProvider::get()->nu->CreateTapsigner(makeTransport(env, iso_dep));
        auto signer = NunchukProvider::get()->nu->CreateTapsignerMasterSigner(
                ts.get(),
                env->GetStringUTFChars(cvc, JNI_FALSE),
                env->GetStringUTFChars(name, JNI_FALSE),
                [](int percent) { return true; }
        );

        return Deserializer::convert2JMasterSigner(env, signer);
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
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_signTransactionByTapSigner(JNIEnv *env,
                                                                                jobject thiz,
                                                                                jobject iso_dep,
                                                                                jstring cvc,
                                                                                jstring wallet_id,
                                                                                jstring tx_id) {
    try {
        auto ts = NunchukProvider::get()->nu->CreateTapsigner(makeTransport(env, iso_dep));
        auto transaction = NunchukProvider::get()->nu->SignTapsignerTransaction(
                ts.get(),
                env->GetStringUTFChars(cvc, JNI_FALSE),
                env->GetStringUTFChars(wallet_id, JNI_FALSE),
                env->GetStringUTFChars(tx_id, JNI_FALSE)
        );

        return Deserializer::convert2JTransaction(env, transaction);
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return JNI_FALSE;
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return JNI_FALSE;
    }
}
