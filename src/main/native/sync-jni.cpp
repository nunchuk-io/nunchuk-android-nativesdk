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

                    JNIEnv *g_env;
                    try {
                        JavaVMAttachArgs args;
                        args.version = JNI_VERSION_1_6;
                        args.name = nullptr;
                        args.group = nullptr;
                        Initializer::get()->jvm->GetEnv((void **) &g_env, JNI_VERSION_1_6);
                        int envState = Initializer::get()->jvm->GetEnv((void **) &g_env, JNI_VERSION_1_6);
                        if (envState == JNI_EDETACHED) {
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
                        syslog(LOG_DEBUG, "[JNI]percent: %d", percent);

                        g_env->CallStaticVoidMethod(
                                Initializer::get()->syncFileClass,
                                Initializer::get()->syncFileMethod,
                                percent == 100,
                                percent
                        );
                    } catch (const std::exception &t) {
                        Deserializer::convertStdException2JException(g_env, t);
                        g_env->ExceptionOccurred();
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
        auto* buf = new unsigned char[len];
        env->GetByteArrayRegion (file_data, 0, len, reinterpret_cast<jbyte*>(buf));

        std::vector<unsigned char> data;
        data.reserve(len);
        for (int i = 0; i < len; i++) {
            data.push_back(buf[i]);
        }

        NunchukProvider::get()->nuMatrix->DownloadFileCallback(
                NunchukProvider::get()->nu,
                env->GetStringUTFChars(file_json_info, JNI_FALSE),
                data,
                [&](int percent) {
                    syslog(LOG_DEBUG, "[JNI]consumeSyncFile percent() :%d",percent);
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
                                Initializer::get()->syncFileClass,
                                Initializer::get()->syncFileMethod,
                                percent == 100,
                                percent
                        );
                    } catch (const std::exception &t) {
                        Deserializer::convertStdException2JException(g_env, t);
                        g_env->ExceptionOccurred();
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
                env->GetStringUTFChars(file_json_info, JNI_FALSE),
                env->GetStringUTFChars(file_path, JNI_FALSE),
                [&](int percent) {
                    syslog(LOG_DEBUG, "[JNI]consumeSyncFile percent() :%d",percent);
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
                                Initializer::get()->syncFileClass,
                                Initializer::get()->syncFileMethod,
                                percent == 100,
                                percent
                        );
                    } catch (const std::exception &t) {
                        Deserializer::convertStdException2JException(g_env, t);
                        g_env->ExceptionOccurred();
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
        jstring sync_room_id,
        jstring access_token
) {
    syslog(LOG_DEBUG, "[JNI] enableAutoBackUp()");
    try {

        env->GetJavaVM(&Initializer::get()->jvm);

        NunchukProvider::get()->nuMatrix->EnableAutoBackup(
                NunchukProvider::get()->nu,
                env->GetStringUTFChars(sync_room_id, JNI_FALSE),
                env->GetStringUTFChars(access_token, JNI_FALSE)
        );

    } catch (std::exception &e) {
        syslog(LOG_DEBUG, "[JNI] enableAutoBackUp error::%s", e.what());
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
                env->GetStringUTFChars(file_json_info, JNI_FALSE),
                env->GetStringUTFChars(file_url, JNI_FALSE)
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

                        jbyteArray byteData = g_env->NewByteArray((int) data_length);
                        g_env->SetByteArrayRegion(byteData, 0, (int) data_length, (jbyte *) (data));


                        g_env->CallStaticVoidMethod(
                                Initializer::get()->sendFileClass,
                                Initializer::get()->sendFileMethod,
                                g_env->NewStringUTF(file_name.c_str()),
                                g_env->NewStringUTF(mine_type.c_str()),
                                g_env->NewStringUTF(file_json_info.c_str()),
                                byteData,
                                (int) data_length
                        );
                    } catch (const std::exception &t) {
                        Deserializer::convertStdException2JException(g_env, t);
                        g_env->ExceptionOccurred();
                    }
                    return "";
                },
                [&](const std::string &file_name, const std::string &mine_type, const std::string &json_info,
                    const std::string &mxc_uri) {

                    JNIEnv *g_env;
                    try {
                        JavaVMAttachArgs args;
                        args.version = JNI_VERSION_1_6;
                        args.name = nullptr;
                        args.group = nullptr;
                        Initializer::get()->jvm->GetEnv((void **) &g_env, JNI_VERSION_1_6);
                        int envState = Initializer::get()->jvm->GetEnv((void **) &g_env, JNI_VERSION_1_6);
                        if (envState == JNI_EDETACHED) {
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
                                Initializer::get()->downloadFileClass,
                                Initializer::get()->downloadFileMethod,
                                g_env->NewStringUTF(file_name.c_str()),
                                g_env->NewStringUTF(mine_type.c_str()),
                                g_env->NewStringUTF(json_info.c_str()),
                                g_env->NewStringUTF(mxc_uri.c_str())
                        );
                    } catch (const std::exception &t) {
                        Deserializer::convertStdException2JException(g_env, t);
                        g_env->ExceptionOccurred();
                    }

                    std::vector<unsigned char> vect;
                    vect.push_back('0');
                    return vect;
                }
        );

    } catch (std::exception &e) {
        syslog(LOG_DEBUG, "[JNI] registerDownloadFileBackup error::%s", e.what());
        Deserializer::convertStdException2JException(env, e);
        env->ExceptionOccurred();
    }
}
