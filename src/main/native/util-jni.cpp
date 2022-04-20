#include <cstring>
#include <jni.h>
#include <syslog.h>
#include <nunchuk.h>
#include "nunchukprovider.h"
#include "serializer.h"
#include "deserializer.h"
#include "initializer.h"

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
JNIEXPORT void JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_addBlockchainConnectionListener(
        JNIEnv *env,
        jobject thiz
) {
    syslog(LOG_DEBUG, "[JNI]addBlockchainConnectionListener()");

    try {

        env->GetJavaVM(&Initializer::get()->jvm);

        NunchukProvider::get()->nu->AddBlockchainConnectionListener(
                [&](ConnectionStatus connectionStatus, int percent) {

                    JNIEnv *g_env;
                    try {
                        JavaVMAttachArgs args;
                        args.version = JNI_VERSION_1_6;
                        args.name = nullptr;
                        args.group = nullptr;
                        Initializer::get()->jvm->GetEnv((void **) &g_env, JNI_VERSION_1_6);
                        int envState = Initializer::get()->jvm->GetEnv((void **) &g_env, JNI_VERSION_1_6);
                        if (envState == JNI_EDETACHED) {
                            syslog(LOG_DEBUG, "[JNI] GetEnv: not attached\n");
                            if (Initializer::get()->jvm->AttachCurrentThread(&g_env, &args) != 0) {
                                syslog(LOG_DEBUG, "[JNI] GetEnv: Failed to attach\n");
                            } else {
                                syslog(LOG_DEBUG, "[JNI] GetEnv: Attached to current thread\n");
                            }
                        } else if (envState == JNI_OK) {
                            syslog(LOG_DEBUG, "[JNI] GetEnv: JNI_OK\n");
                        } else if (envState == JNI_EVERSION) {
                            syslog(LOG_DEBUG, "[JNI] GetEnv: version not supported\n");
                        }

                        g_env->CallStaticVoidMethod(
                                Initializer::get()->connectStatusClass,
                                Initializer::get()->connectStatusMethod,
                                (int) connectionStatus,
                                percent
                        );
                    } catch (const std::exception &t) {
                        Deserializer::convertStdException2JException(g_env, t);
                        g_env->ExceptionOccurred();
                    }
                }
        );

    } catch (std::exception &e) {
        syslog(LOG_DEBUG, "[JNI] addBlockchainConnectionListener error::%s", e.what());
        Deserializer::convertStdException2JException(env, e);
        env->ExceptionOccurred();
    }
}

