#include <cstring>
#include <jni.h>
#include <nunchuk.h>
#include "nunchukprovider.h"
#include "serializer.h"
#include "initializer.h"
#include "deserializer.h"

using namespace nunchuk;

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env;
    vm->GetEnv((void **) &env, JNI_VERSION_1_6);

    // Store Send Event
    jclass tmpSendEventClass = env->FindClass("com/nunchuk/android/model/SendEventHelper");
    jmethodID tmpSendEventMethod = env->GetStaticMethodID(tmpSendEventClass, "sendEvent", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Z)V");
    Initializer::get()->sendEventClass = (jclass) env->NewGlobalRef(tmpSendEventClass);
    Initializer::get()->senEventMethod = tmpSendEventMethod;

    // Store Upload File
    jclass tmpSendFileClass = env->FindClass("com/nunchuk/android/model/SyncFileEventHelper");
    jmethodID tmpSendFileMethod = env->GetStaticMethodID(tmpSendFileClass, "uploadFileEvent", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;[BI)V");
    Initializer::get()->sendFileClass = (jclass) env->NewGlobalRef(tmpSendFileClass);
    Initializer::get()->sendFileMethod = tmpSendFileMethod;

    // Store RegisterDownloadFile
    jclass tmpDownloadFileClass = env->FindClass("com/nunchuk/android/model/SyncFileEventHelper");
    jmethodID tmpDownloadFileMethod = env->GetStaticMethodID(tmpDownloadFileClass, "downloadFileEvent", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
    Initializer::get()->downloadFileClass = (jclass) env->NewGlobalRef(tmpDownloadFileClass);
    Initializer::get()->downloadFileMethod = tmpDownloadFileMethod;

    // Store SyncFile
    jclass tmpSyncFileClass = env->FindClass("com/nunchuk/android/model/SyncFileEventHelper");
    jmethodID tmpSyncFileMethod = env->GetStaticMethodID(tmpSyncFileClass, "syncFileEvent", "(ZI)V");
    Initializer::get()->syncFileClass = (jclass) env->NewGlobalRef(tmpSyncFileClass);
    Initializer::get()->syncFileMethod = tmpSyncFileMethod;

    // Store ConnectStatus
    jclass tmpConnectionClass = env->FindClass("com/nunchuk/android/model/ConnectionStatusHelper");
    jmethodID tmpConnectionMethod = env->GetStaticMethodID(tmpConnectionClass, "addListener", "(II)V");
    Initializer::get()->connectStatusClass = (jclass) env->NewGlobalRef(tmpConnectionClass);
    Initializer::get()->connectStatusMethod = tmpConnectionMethod;
    return JNI_VERSION_1_6;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_initNunchuk(
        JNIEnv *env,
        jobject thiz,
        jint chain,
        jstring hwi_path,
        jboolean enable_proxy,
        jobject testnet_servers,
        jobject mainnet_servers,
        jobject signet_servers,
        jint backend_type,
        jstring storage_path,
        jstring pass_phrase,
        jstring account_id
) {
    try {
        AppSettings settings;
        settings.set_chain(Serializer::convert2CChain(chain));
        settings.set_hwi_path(env->GetStringUTFChars(hwi_path, JNI_FALSE));
        settings.enable_proxy(enable_proxy);
        settings.set_testnet_servers(Serializer::convert2CListString(env, testnet_servers));
        settings.set_mainnet_servers(Serializer::convert2CListString(env, mainnet_servers));
        settings.set_signet_servers(Serializer::convert2CListString(env, signet_servers));
        settings.set_backend_type(Serializer::convert2CBackendType(backend_type));
        settings.set_storage_path(env->GetStringUTFChars(storage_path, JNI_FALSE));

        env->GetJavaVM(&Initializer::get()->jvm);
        NunchukProvider::get()->initNunchuk(
                settings,
                env->GetStringUTFChars(pass_phrase, JNI_FALSE),
                env->GetStringUTFChars(account_id, JNI_FALSE),
                [](const std::string &room_id, const std::string &type, const std::string &content, bool ignore_error) {
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
                                syslog(LOG_DEBUG, "[JNI]GetEnv: Failed to attach\n");
                            } else {
                                syslog(LOG_DEBUG, "[JNI]GetEnv: Attached to current thread\n");
                            }
                        } else if (envState == JNI_OK) {
                            syslog(LOG_DEBUG, "[JNI]GetEnv: JNI_OK\n");
                        } else if (envState == JNI_EVERSION) {
                            syslog(LOG_DEBUG, "[JNI]GetEnv: version not supported\n");
                        }
                        g_env->CallStaticVoidMethod(
                                Initializer::get()->sendEventClass,
                                Initializer::get()->senEventMethod,
                                g_env->NewStringUTF(room_id.c_str()),
                                g_env->NewStringUTF(type.c_str()),
                                g_env->NewStringUTF(content.c_str()),
                                ignore_error
                        );
                    } catch (const std::exception &t) {
                        Deserializer::convert2JException(g_env, t.what());
                        g_env->ExceptionOccurred();
                    }
                    return "";
                }
        );
        try {
            NunchukProvider::get()->nu->AddBlockchainConnectionListener(
                    [](ConnectionStatus connectionStatus, int percent) {
                        syslog(LOG_DEBUG, "[JNI] addBlockchainConnectionListener percent::%d", percent);
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
                                    Initializer::get()->connectStatusClass,
                                    Initializer::get()->connectStatusMethod,
                                    (int) connectionStatus,
                                    percent
                            );
                        } catch (const std::exception &t) {
                            Deserializer::convert2JException(g_env, t.what());
                            g_env->ExceptionOccurred();
                        }
                    }
            );

        } catch (std::exception &e) {
            syslog(LOG_DEBUG, "[JNI] addBlockchainConnectionListener error::%s", e.what());
            Deserializer::convert2JException(env, e.what());
            env->ExceptionOccurred();
        }
        NunchukProvider::get()->nuMatrix->EnableGenerateReceiveEvent(NunchukProvider::get()->nu);
    } catch (const std::exception &e) {
        Deserializer::convert2JException(env, e.what());
        env->ExceptionOccurred();
    }
}
