#include <cstring>
#include <jni.h>
#include <nunchuk.h>
#include <nunchukmatrix.h>
#include "nunchukprovider.h"
#include "initializer.h"
#include "serializer.h"
#include "deserializer.h"
#include "string-wrapper.h"

using namespace nunchuk;

extern "C"
JNIEXPORT void JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_consumeSyncEvent(
        JNIEnv *env,
        jobject thiz,
        jobject event
) {
    syslog(LOG_DEBUG, "[JNI] consumeSyncEvent()");
    try {
        auto matrixEvent = Serializer::convert2CMatrixEvent(env, event);
        env->GetJavaVM(&Initializer::get()->jvm);

        NunchukProvider::get()->nuMatrix->ConsumeSyncEvent(
                NunchukProvider::get()->nu,
                matrixEvent,
                [&](int percent) {
                    JNIEnvGuard guard;
                    if (!guard) return percent == 100;
                    JNIEnv *g_env = guard.get();
                    try {
                        g_env->CallStaticVoidMethod(
                                Initializer::get()->syncFileClass,
                                Initializer::get()->syncFileMethod,
                                percent == 100,
                                percent
                        );
                    } catch (const std::exception &t) {
                        syslog(LOG_DEBUG, "[JNI] consumeSyncEvent callback error::%s", t.what());
                    }
                    return percent == 100;
                }
        );

    } catch (std::exception &e) {
        syslog(LOG_DEBUG, "[JNI] consumeSyncEvent error::%s", e.what());
        Deserializer::convertStdException2JException(env, e);
        env->ExceptionOccurred();
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_downloadFileCallback(
        JNIEnv *env,
        jobject thiz,
        jstring file_json_info,
        jbyteArray file_data
) {
    syslog(LOG_DEBUG, "[JNI] DownloadFileCallback()");

    try {

        env->GetJavaVM(&Initializer::get()->jvm);

        int len = env->GetArrayLength (file_data);
        std::vector<unsigned char> data(len);
        env->GetByteArrayRegion (file_data, 0, len, reinterpret_cast<jbyte*>(data.data()));

        NunchukProvider::get()->nuMatrix->DownloadFileCallback(
                NunchukProvider::get()->nu,
                StringWrapper(env, file_json_info),
                data,
                [&](int percent) {
                    JNIEnvGuard guard;
                    if (!guard) return percent == 100;
                    JNIEnv *g_env = guard.get();
                    try {
                        g_env->CallStaticVoidMethod(
                                Initializer::get()->syncFileClass,
                                Initializer::get()->syncFileMethod,
                                percent == 100,
                                percent
                        );
                    } catch (const std::exception &t) {
                        syslog(LOG_DEBUG, "[JNI] downloadFileCallback error::%s", t.what());
                    }
                    return percent == 100;
                }
        );
    } catch (std::exception &e) {
        syslog(LOG_DEBUG, "[JNI] consumeSyncFile error::%s", e.what());
        Deserializer::convertStdException2JException(env, e);
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_writeFileCallback(
        JNIEnv *env,
        jobject thiz,
        jstring file_json_info,
        jstring file_path
) {
    syslog(LOG_DEBUG, "[JNI] WriteFileCallback()");

    try {

        env->GetJavaVM(&Initializer::get()->jvm);

        NunchukProvider::get()->nuMatrix->WriteFileCallback(
                NunchukProvider::get()->nu,
                StringWrapper(env, file_json_info),
                StringWrapper(env, file_path),
                [&](int percent) {
                    JNIEnvGuard guard;
                    if (!guard) return percent == 100;
                    JNIEnv *g_env = guard.get();
                    try {
                        g_env->CallStaticVoidMethod(
                                Initializer::get()->syncFileClass,
                                Initializer::get()->syncFileMethod,
                                percent == 100,
                                percent
                        );
                    } catch (const std::exception &t) {
                        syslog(LOG_DEBUG, "[JNI] writeFileCallback error::%s", t.what());
                    }
                    return percent == 100;
                }
        );
    } catch (std::exception &e) {
        syslog(LOG_DEBUG, "[JNI] consumeSyncFile error::%s", e.what());
        Deserializer::convertStdException2JException(env, e);
        env->ExceptionOccurred();
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_enableAutoBackUp(
        JNIEnv *env,
        jobject thiz,
        jboolean enable

) {
    syslog(LOG_DEBUG, "[JNI] enableAutoBackUp()");
    try {
        env->GetJavaVM(&Initializer::get()->jvm);

        NunchukProvider::get()->nuMatrix->EnableAutoBackup(
                enable
        );

    } catch (std::exception &e) {
        syslog(LOG_DEBUG, "[JNI] enableAutoBackUp error::%s", e.what());
        Deserializer::convertStdException2JException(env, e);
        env->ExceptionOccurred();
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_registerAutoBackUp(
        JNIEnv *env,
        jobject thiz,
        jstring sync_room_id,
        jstring access_token
) {
    syslog(LOG_DEBUG, "[JNI] RegisterAutoBackup()");
    try {

        env->GetJavaVM(&Initializer::get()->jvm);

        NunchukProvider::get()->nuMatrix->RegisterAutoBackup(
                NunchukProvider::get()->nu,
                StringWrapper(env, sync_room_id),
                StringWrapper(env, access_token)
        );

    } catch (std::exception &e) {
        syslog(LOG_DEBUG, "[JNI] RegisterAutoBackup error::%s", e.what());
        Deserializer::convertStdException2JException(env, e);
        env->ExceptionOccurred();
    }
}


extern "C"
JNIEXPORT void JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_backup(
        JNIEnv *env,
        jobject thiz
) {
    syslog(LOG_DEBUG, "[JNI] backUp()");
    try {

        env->GetJavaVM(&Initializer::get()->jvm);

        NunchukProvider::get()->nuMatrix->Backup(
                NunchukProvider::get()->nu
        );

    } catch (std::exception &e) {
        syslog(LOG_DEBUG, "[JNI] backUp error::%s", e.what());
        Deserializer::convertStdException2JException(env, e);
        env->ExceptionOccurred();
    }
}


extern "C"
JNIEXPORT void JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_uploadFileCallback(
        JNIEnv *env,
        jobject thiz,
        jstring file_json_info,
        jstring file_url
) {
    syslog(LOG_DEBUG, "[JNI]UploadFileCallback()");
    try {
        NunchukProvider::get()->nuMatrix->UploadFileCallback(
                StringWrapper(env, file_json_info),
                StringWrapper(env, file_url)
        );
    } catch (std::exception &e) {
        syslog(LOG_DEBUG, "[JNI] UploadFileCallback error::%s", e.what());
        Deserializer::convertStdException2JException(env, e);
        env->ExceptionOccurred();
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_registerDownloadFileBackup(
        JNIEnv *env,
        jobject thiz
) {
    syslog(LOG_DEBUG, "[JNI] registerDownloadFileBackup()");
    try {

        env->GetJavaVM(&Initializer::get()->jvm);

        NunchukProvider::get()->nuMatrix->RegisterFileFunc(
                [&](const std::string &file_name, const std::string &mine_type, const std::string &file_json_info,
                    const char *data, size_t data_length) {
                    JNIEnvGuard guard;
                    if (!guard) return std::string("");
                    JNIEnv *g_env = guard.get();
                    try {
                        jbyteArray byteData = g_env->NewByteArray((int) data_length);
                        g_env->SetByteArrayRegion(byteData, 0, (int) data_length, (jbyte *) (data));

                        jstring jFileName = g_env->NewStringUTF(file_name.c_str());
                        jstring jMineType = g_env->NewStringUTF(mine_type.c_str());
                        jstring jFileJsonInfo = g_env->NewStringUTF(file_json_info.c_str());
                        g_env->CallStaticVoidMethod(
                                Initializer::get()->sendFileClass,
                                Initializer::get()->sendFileMethod,
                                jFileName, jMineType, jFileJsonInfo,
                                byteData, (int) data_length
                        );
                        g_env->DeleteLocalRef(jFileName);
                        g_env->DeleteLocalRef(jMineType);
                        g_env->DeleteLocalRef(jFileJsonInfo);
                        g_env->DeleteLocalRef(byteData);
                    } catch (const std::exception &t) {
                        syslog(LOG_DEBUG, "[JNI] uploadFile error::%s", t.what());
                    }
                    return std::string("");
                },
                [&](const std::string &file_name, const std::string &mine_type, const std::string &json_info,
                    const std::string &mxc_uri) {
                    JNIEnvGuard guard;
                    if (!guard) return std::vector<unsigned char>{'0'};
                    JNIEnv *g_env = guard.get();
                    try {
                        jstring jFileName = g_env->NewStringUTF(file_name.c_str());
                        jstring jMineType = g_env->NewStringUTF(mine_type.c_str());
                        jstring jJsonInfo = g_env->NewStringUTF(json_info.c_str());
                        jstring jMxcUri = g_env->NewStringUTF(mxc_uri.c_str());
                        g_env->CallStaticVoidMethod(
                                Initializer::get()->downloadFileClass,
                                Initializer::get()->downloadFileMethod,
                                jFileName, jMineType, jJsonInfo, jMxcUri
                        );
                        g_env->DeleteLocalRef(jFileName);
                        g_env->DeleteLocalRef(jMineType);
                        g_env->DeleteLocalRef(jJsonInfo);
                        g_env->DeleteLocalRef(jMxcUri);
                    } catch (const std::exception &t) {
                        syslog(LOG_DEBUG, "[JNI] downloadFile error::%s", t.what());
                    }
                    return std::vector<unsigned char>{'0'};
                }
        );

    } catch (std::exception &e) {
        syslog(LOG_DEBUG, "[JNI] registerDownloadFileBackup error::%s", e.what());
        Deserializer::convertStdException2JException(env, e);
        env->ExceptionOccurred();
    }
}
