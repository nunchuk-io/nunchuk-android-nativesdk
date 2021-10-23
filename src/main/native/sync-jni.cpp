#include <cstring>
#include <jni.h>
#include <nunchuk.h>
#include <nunchukmatrix.h>
#include "nunchukprovider.h"
#include "initializer.h"
#include "serializer.h"
#include "deserializer.h"

using namespace nunchuk;

extern "C"
JNIEXPORT void JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_consumeSyncEvent(
        JNIEnv *env,
        jobject thiz,
        jobject event,
        jobject progress
) {
    syslog(LOG_DEBUG, "[JNI]consumeSyncEvent()");
    try {
        auto matrixEvent = Serializer::convert2CMatrixEvent(env, event);
        jclass callbackClass = env->GetObjectClass(progress);
        jmethodID methodId = env->GetMethodID(callbackClass, "onSyncProgress", "(ZI)V");
        NunchukProvider::get()->nuMatrix->ConsumeSyncEvent(
                NunchukProvider::get()->nu,
                matrixEvent, [&env, callbackClass, methodId](int percent) {
                    env->CallVoidMethod(callbackClass, methodId, percent == 100, percent);
                    return true;
                }
        );
    } catch (std::exception &e) {
        syslog(LOG_DEBUG, "[JNI] consumeSyncEvent error::%s", e.what());
        Deserializer::convert2JException(env, e.what());
        env->ExceptionOccurred();
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_enableAutoBackUp(
        JNIEnv *env,
        jobject thiz,
        jstring sync_room_id
) {
    syslog(LOG_DEBUG, "[JNI] enableAutoBackUp()");
    try {

        env->GetJavaVM(&Initializer::get()->jvm);

        NunchukProvider::get()->nuMatrix->EnableAutoBackup(
                NunchukProvider::get()->nu,
                env->GetStringUTFChars(sync_room_id, JNI_FALSE),
                [&](const std::string &file_name, const std::string &mine_type, const std::string &file_json_info,
                    const char *data, size_t data_length) {
                    syslog(LOG_DEBUG, "[JNI] enableAutoBackUp file_name::%s", file_name.c_str());
                    syslog(LOG_DEBUG, "[JNI] enableAutoBackUp file_json_info::%s", file_json_info.c_str());
                    syslog(LOG_DEBUG, "[JNI] enableAutoBackUp mine_type::%s", mine_type.c_str());
                    syslog(LOG_DEBUG, "[JNI] enableAutoBackUp data_length::%d", (int) data_length);
                    syslog(LOG_DEBUG, "[JNI] enableAutoBackUp data::%s", data);
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
                        syslog(LOG_DEBUG, "[JNI]CallStaticVoidMethod");

                        g_env->CallStaticVoidMethod(
                                Initializer::get()->sendFileClass,
                                Initializer::get()->sendFileMethod,
                                g_env->NewStringUTF(file_name.c_str()),
                                g_env->NewStringUTF(mine_type.c_str()),
                                g_env->NewStringUTF(file_json_info.c_str()),
                                (int) data_length
                        );
                    } catch (const std::exception &t) {
                        Deserializer::convert2JException(g_env, t.what());
                        g_env->ExceptionOccurred();
                    }
                    return "";
                }
        );

    } catch (std::exception &e) {
        syslog(LOG_DEBUG, "[JNI] enableAutoBackUp error::%s", e.what());
        Deserializer::convert2JException(env, e.what());
        env->ExceptionOccurred();
    }
}

