#include <cstring>
#include <jni.h>
#include <syslog.h>
#include <nunchuk.h>
#include <iostream>
#include <sstream>
#include <string>
#include "nfc.h"
#include "nunchukprovider.h"
#include "deserializer.h"

using namespace nunchuk;

std::unique_ptr<tap_protocol::Transport> NFC::makeTransport(JNIEnv *env, jobject iso_dep) {
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
        env->DeleteLocalRef(bytesToSend);

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
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_getAutoCardStatus(JNIEnv *env, jobject thiz, jobject iso_dep) {
    try {
        auto card = NunchukProvider::get()->nu->CreateCKTapCard(NFC::makeTransport(env, iso_dep));
        if (card->IsTapsigner()) {
            auto ts = tap_protocol::ToTapsigner(std::move(*card));
            auto status = NunchukProvider::get()->nu->GetTapsignerStatus(ts.get());
            return Deserializer::convert2JTapSignerStatus(env, status);
        } else {
            auto sats = tap_protocol::ToSatscard(std::move(*card));
            auto status = NunchukProvider::get()->nu->GetSatscardStatus(sats.get());
            return Deserializer::convert2JSatsCardStatus(env, status);
        }
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
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_waitAutoCard(JNIEnv *env, jobject thiz, jobject iso_dep) {
    try {
        auto ts = NunchukProvider::get()->nu->CreateCKTapCard(NFC::makeTransport(env, iso_dep));
        NunchukProvider::get()->nu->WaitCKTapCard(
                ts.get(),
                [](int percent) { return true; }
        );
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        env->ExceptionOccurred();
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        env->ExceptionOccurred();
    }
}