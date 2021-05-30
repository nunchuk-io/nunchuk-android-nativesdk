#include <cstring>
#include <jni.h>
#include <syslog.h>
#include <nunchuk.h>
#include "provider.h"
#include "serializer.h"
#include "deserializer.h"

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
    } catch (std::exception &e) {
        syslog(LOG_DEBUG, "[JNI] valueFromAmount error::%s", e.what());
        Deserializer::convert2JException(env, e.what());
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
    } catch (std::exception &e) {
        Deserializer::convert2JException(env, e.what());
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
    return NunchukProvider::get()->nu->GetChainTip();
}

