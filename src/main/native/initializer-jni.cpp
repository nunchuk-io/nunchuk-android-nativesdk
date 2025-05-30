#include <cstring>
#include <jni.h>
#include <nunchuk.h>
#include "nunchukprovider.h"
#include "serializer.h"
#include "initializer.h"
#include "deserializer.h"
#include "string-wrapper.h"

using namespace nunchuk;

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env;
    vm->GetEnv((void **) &env, JNI_VERSION_1_6);

    // Store Send Event
    jclass tmpSendEventClass = env->FindClass("com/nunchuk/android/model/SendEventHelper");
    jmethodID tmpSendEventMethod = env->GetStaticMethodID(tmpSendEventClass, "sendEvent",
                                                          "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Z)V");
    Initializer::get()->sendEventClass = (jclass) env->NewGlobalRef(tmpSendEventClass);
    Initializer::get()->senEventMethod = tmpSendEventMethod;

    // Store Upload File
    jclass tmpSendFileClass = env->FindClass("com/nunchuk/android/model/SyncFileEventHelper");
    jmethodID tmpSendFileMethod = env->GetStaticMethodID(tmpSendFileClass, "uploadFileEvent",
                                                         "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;[BI)V");
    Initializer::get()->sendFileClass = (jclass) env->NewGlobalRef(tmpSendFileClass);
    Initializer::get()->sendFileMethod = tmpSendFileMethod;

    // Store RegisterDownloadFile
    jclass tmpDownloadFileClass = env->FindClass("com/nunchuk/android/model/SyncFileEventHelper");
    jmethodID tmpDownloadFileMethod = env->GetStaticMethodID(tmpDownloadFileClass,
                                                             "downloadFileEvent",
                                                             "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
    Initializer::get()->downloadFileClass = (jclass) env->NewGlobalRef(tmpDownloadFileClass);
    Initializer::get()->downloadFileMethod = tmpDownloadFileMethod;

    // Store SyncFile
    jclass tmpSyncFileClass = env->FindClass("com/nunchuk/android/model/SyncFileEventHelper");
    jmethodID tmpSyncFileMethod = env->GetStaticMethodID(tmpSyncFileClass, "syncFileEvent",
                                                         "(ZI)V");
    Initializer::get()->syncFileClass = (jclass) env->NewGlobalRef(tmpSyncFileClass);
    Initializer::get()->syncFileMethod = tmpSyncFileMethod;

    // Store ConnectStatus
    jclass tmpConnectionClass = env->FindClass("com/nunchuk/android/model/ConnectionStatusHelper");
    jmethodID tmpConnectionMethod = env->GetStaticMethodID(tmpConnectionClass, "addListener",
                                                           "(II)V");
    Initializer::get()->connectStatusClass = (jclass) env->NewGlobalRef(tmpConnectionClass);
    Initializer::get()->connectStatusMethod = tmpConnectionMethod;

    auto tmpBlockListenerClass = env->FindClass("com/nunchuk/android/listener/BlockListener");
    auto tmpBlockListenerMethod = env->GetStaticMethodID(tmpBlockListenerClass, "onBlockUpdate",
                                                         "(ILjava/lang/String;)V");
    Initializer::get()->blockListenerClass = (jclass) env->NewGlobalRef(tmpBlockListenerClass);
    Initializer::get()->blockListenerMethod = tmpBlockListenerMethod;

    auto tmpTransactionListenerClass = env->FindClass(
            "com/nunchuk/android/listener/TransactionListener");
    auto tmpTransactionListenerMethod = env->GetStaticMethodID(tmpTransactionListenerClass,
                                                               "onTransactionUpdate",
                                                               "(Ljava/lang/String;Ljava/lang/String;)V");
    Initializer::get()->transactionListenerClass = (jclass) env->NewGlobalRef(
            tmpTransactionListenerClass);
    Initializer::get()->transactionListenerMethod = tmpTransactionListenerMethod;

    auto tmpGroupSandboxListenerClass = env->FindClass(
            "com/nunchuk/android/listener/GroupSandboxListener");
    auto tmpGroupMessageListenerClass = env->FindClass(
            "com/nunchuk/android/listener/GroupMessageListener");
    auto tmpGroupSandboxListenerMethod = env->GetStaticMethodID(tmpGroupSandboxListenerClass,
                                                                "onGroupUpdate",
                                                                "(Lcom/nunchuk/android/model/GroupSandbox;)V");
    auto tmpGroupMessageListenerMethod = env->GetStaticMethodID(tmpGroupMessageListenerClass,
                                                                "onGroupMessageUpdate",
                                                                "(Lcom/nunchuk/android/model/FreeGroupMessage;)V");
    Initializer::get()->groupSandboxListenerClass = (jclass) env->NewGlobalRef(
            tmpGroupSandboxListenerClass);
    Initializer::get()->groupSandboxListenerMethod = tmpGroupSandboxListenerMethod;
    Initializer::get()->groupMessageListenerClass = (jclass) env->NewGlobalRef(
            tmpGroupMessageListenerClass);
    Initializer::get()->groupMessageListenerMethod = tmpGroupMessageListenerMethod;

    auto tmpGroupOnlineListenerClass = env->FindClass(
            "com/nunchuk/android/listener/GroupOnlineListener");
    auto tmpGroupOnlineListenerMethod = env->GetStaticMethodID(tmpGroupOnlineListenerClass,
                                                               "onGroupOnlineUpdate",
                                                               "(Ljava/lang/String;I)V");

    Initializer::get()->groupOnlineListenerClass = (jclass) env->NewGlobalRef(
            tmpGroupOnlineListenerClass);
    Initializer::get()->groupOnlineListenerMethod = tmpGroupOnlineListenerMethod;

    auto tmpGroupDeleteListenerClass = env->FindClass("com/nunchuk/android/listener/GroupDeleteListener");
    auto tmpGroupDeleteListenerMethod = env->GetStaticMethodID(tmpGroupDeleteListenerClass, "onGroupDelete", "(Ljava/lang/String;)V");

    Initializer::get()->groupDeleteListenerClass = (jclass) env->NewGlobalRef(tmpGroupDeleteListenerClass);
    Initializer::get()->groupDeleteListenerMethod = tmpGroupDeleteListenerMethod;

    auto tmpGroupUpdateListenerClass = env->FindClass("com/nunchuk/android/listener/GroupReplaceListener");
    auto tmpGroupUpdateListenerMethod = env->GetStaticMethodID(tmpGroupUpdateListenerClass, "onGroupReplace", "(Ljava/lang/String;Ljava/lang/String;)V");
    Initializer::get()->groupReplaceListenerClass = (jclass) env->NewGlobalRef(tmpGroupUpdateListenerClass);
    Initializer::get()->groupReplaceListenerMethod = tmpGroupUpdateListenerMethod;

    // Cache the ClassLoader reference
    jclass threadClass = env->FindClass("java/lang/Thread");
    jmethodID currentThreadMethod = env->GetStaticMethodID(threadClass, "currentThread", "()Ljava/lang/Thread;");
    jobject currentThread = env->CallStaticObjectMethod(threadClass, currentThreadMethod);
    jmethodID getContextClassLoaderMethod = env->GetMethodID(threadClass, "getContextClassLoader", "()Ljava/lang/ClassLoader;");
    jobject localClassLoader = env->CallObjectMethod(currentThread, getContextClassLoaderMethod);
    Initializer::get()->classLoader = env->NewGlobalRef(localClassLoader);
    Initializer::get()->loadClassMethod = env->GetMethodID(env->FindClass("java/lang/ClassLoader"), "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");


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
        jstring account_id,
        jstring device_id,
        jstring decoy_pin,
        jstring base_url_api
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
        settings.set_group_server(env->GetStringUTFChars(base_url_api, JNI_FALSE));

        env->GetJavaVM(&Initializer::get()->jvm);
        NunchukProvider::get()->initNunchuk(
                settings,
                StringWrapper(env, pass_phrase),
                StringWrapper(env, account_id),
                StringWrapper(env, device_id),
                StringWrapper(env, decoy_pin),
                [](const std::string &room_id, const std::string &type, const std::string &content,
                   bool ignore_error) {
                    JNIEnv *g_env;
                    try {
                        JavaVMAttachArgs args;
                        args.version = JNI_VERSION_1_6;
                        args.name = nullptr;
                        args.group = nullptr;
                        Initializer::get()->jvm->GetEnv((void **) &g_env, JNI_VERSION_1_6);
                        int envState = Initializer::get()->jvm->GetEnv((void **) &g_env,
                                                                       JNI_VERSION_1_6);
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
                        Deserializer::convertStdException2JException(g_env, t);
                    }
                    return "";
                }
        );
        try {
            NunchukProvider::get()->nu->AddBlockchainConnectionListener(
                    [](ConnectionStatus connectionStatus, int percent) {
                        syslog(LOG_DEBUG, "[JNI] addBlockchainConnectionListener percent::%d",
                               percent);
                        JNIEnv *g_env;
                        try {
                            JavaVMAttachArgs args;
                            args.version = JNI_VERSION_1_6;
                            args.name = nullptr;
                            args.group = nullptr;
                            Initializer::get()->jvm->GetEnv((void **) &g_env, JNI_VERSION_1_6);
                            int envState = Initializer::get()->jvm->GetEnv((void **) &g_env,
                                                                           JNI_VERSION_1_6);
                            if (envState == JNI_EDETACHED) {
                                if (Initializer::get()->jvm->AttachCurrentThread(&g_env, &args) !=
                                    0) {
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
                        }
                    }
            );
        } catch (BaseException &e) {
            syslog(LOG_DEBUG, "[JNI] addBlockchainConnectionListener error::%s", e.what());
            Deserializer::convert2JException(env, e);
        }

        try {
            NunchukProvider::get()->nu->AddBlockListener(
                    [](int height, const std::string &hex_header) {
                        syslog(LOG_DEBUG, "[JNI] Block listener call\n");
                        JNIEnv *g_env;
                        JavaVMAttachArgs args;
                        args.version = JNI_VERSION_1_6;
                        args.name = nullptr;
                        args.group = nullptr;
                        Initializer::get()->jvm->GetEnv((void **) &g_env, JNI_VERSION_1_6);
                        int envState = Initializer::get()->jvm->GetEnv((void **) &g_env,
                                                                       JNI_VERSION_1_6);
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
                                Initializer::get()->blockListenerClass,
                                Initializer::get()->blockListenerMethod,
                                height,
                                StringWrapper(hex_header).toJString(g_env)
                        );
                    });
        } catch (BaseException &e) {
            syslog(LOG_DEBUG, "[JNI] Block listener error::%s", e.what());
            Deserializer::convert2JException(env, e);
        }

        try {
            NunchukProvider::get()->nu->AddTransactionListener(
                    [](const std::string &tx_id, TransactionStatus status,
                       const std::string &wallet_id) {
                        syslog(LOG_DEBUG, "[JNI] Transaction Listener call\n");
                        JNIEnv *g_env;
                        JavaVMAttachArgs args;
                        args.version = JNI_VERSION_1_6;
                        args.name = nullptr;
                        args.group = nullptr;
                        Initializer::get()->jvm->GetEnv((void **) &g_env, JNI_VERSION_1_6);
                        int envState = Initializer::get()->jvm->GetEnv((void **) &g_env,
                                                                       JNI_VERSION_1_6);
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
                                Initializer::get()->transactionListenerClass,
                                Initializer::get()->transactionListenerMethod,
                                StringWrapper(tx_id).toJString(g_env),
                                StringWrapper(wallet_id).toJString(g_env)
                        );
                    });
        } catch (BaseException &e) {
            syslog(LOG_DEBUG, "[JNI] Transaction Listener Error::%s", e.what());
            Deserializer::convert2JException(env, e);
        }
        NunchukProvider::get()->nuMatrix->EnableGenerateReceiveEvent(NunchukProvider::get()->nu);
    } catch (const std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
    }
}
extern "C"
JNIEXPORT void JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_registerGlobalListener(JNIEnv *env,
                                                                            jobject thiz) {
    try {
        NunchukProvider::get()->nu->AddGroupUpdateListener([](const GroupSandbox &state) {
            syslog(LOG_DEBUG, "[JNI] Group Update Listener call\n");
            JNIEnv *g_env;
            JavaVMAttachArgs args;
            args.version = JNI_VERSION_1_6;
            args.name = nullptr;
            args.group = nullptr;
            Initializer::get()->jvm->GetEnv((void **) &g_env, JNI_VERSION_1_6);
            int envState = Initializer::get()->jvm->GetEnv((void **) &g_env,
                                                           JNI_VERSION_1_6);
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
                    Initializer::get()->groupSandboxListenerClass,
                    Initializer::get()->groupSandboxListenerMethod,
                    Deserializer::convert2JGroupSandbox(g_env, state)
            );
        });
    } catch (BaseException &e) {
        syslog(LOG_DEBUG, "[JNI] Group Update Listener::%s", e.what());
        Deserializer::convert2JException(env, e);
    }

    try {
        NunchukProvider::get()->nu->AddGroupMessageListener([](const GroupMessage &state) {
            syslog(LOG_DEBUG, "[JNI] Group Message Listener call\n");
            JNIEnv *g_env;
            JavaVMAttachArgs args;
            args.version = JNI_VERSION_1_6;
            args.name = nullptr;
            args.group = nullptr;
            Initializer::get()->jvm->GetEnv((void **) &g_env, JNI_VERSION_1_6);
            int envState = Initializer::get()->jvm->GetEnv((void **) &g_env,
                                                           JNI_VERSION_1_6);
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
                    Initializer::get()->groupMessageListenerClass,
                    Initializer::get()->groupMessageListenerMethod,
                    Deserializer::convert2JGroupMessage(g_env, state)
            );
        });
    } catch (BaseException &e) {
        syslog(LOG_DEBUG, "[JNI] Group Message Listener Error::%s", e.what());
        Deserializer::convert2JException(env, e);
    }

    try {
        NunchukProvider::get()->nu->AddGroupOnlineListener(
                [](const std::string &groupId, int online) {
                    syslog(LOG_DEBUG, "[JNI] Group Online Listener call\n");
                    JNIEnv *g_env;
                    JavaVMAttachArgs args;
                    args.version = JNI_VERSION_1_6;
                    args.name = nullptr;
                    args.group = nullptr;
                    Initializer::get()->jvm->GetEnv((void **) &g_env, JNI_VERSION_1_6);
                    int envState = Initializer::get()->jvm->GetEnv((void **) &g_env,
                                                                   JNI_VERSION_1_6);
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
                            Initializer::get()->groupOnlineListenerClass,
                            Initializer::get()->groupOnlineListenerMethod,
                            g_env->NewStringUTF(groupId.c_str()),
                            online
                    );
                });
    } catch (BaseException &e) {
        syslog(LOG_DEBUG, "[JNI] Group Online Listener Error::%s", e.what());
        Deserializer::convert2JException(env, e);
    }

    try {
        NunchukProvider::get()->nu->AddGroupDeleteListener([](const std::string &groupId) {
            syslog(LOG_DEBUG, "[JNI] Group Delete Listener call\n");
            JNIEnv *g_env;
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
                    Initializer::get()->groupDeleteListenerClass,
                    Initializer::get()->groupDeleteListenerMethod,
                    g_env->NewStringUTF(groupId.c_str())
            );
        });
    } catch (BaseException &e) {
        syslog(LOG_DEBUG, "[JNI] Group Delete Listener Error::%s", e.what());
        Deserializer::convert2JException(env, e);
    }

    try {
        NunchukProvider::get()->nu->AddReplaceRequestListener([](const std::string &walletId,
                                                                const std::string &replaceGroupId) {
            syslog(LOG_DEBUG, "[JNI] Group Replace Listener call\n");
            JNIEnv *g_env;
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
                    Initializer::get()->groupReplaceListenerClass,
                    Initializer::get()->groupReplaceListenerMethod,
                    g_env->NewStringUTF(walletId.c_str()),
                    g_env->NewStringUTF(replaceGroupId.c_str())
            );
        });
    } catch (BaseException &e) {
        syslog(LOG_DEBUG, "[JNI] Group Replace Listener Error::%s", e.what());
        Deserializer::convert2JException(env, e);
    }
}

jclass Initializer::getClass(JNIEnv *env, const char *classPath) {
    jstring className = env->NewStringUTF(classPath);
    jclass clazz = static_cast<jclass>(
            env->CallObjectMethod(Initializer::get()->classLoader,
                                  Initializer::get()->loadClassMethod,
                                  className));
    env->DeleteLocalRef(className);
    return clazz;
}
