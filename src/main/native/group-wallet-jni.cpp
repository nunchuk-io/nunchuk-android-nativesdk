#include <jni.h>
#include "nunchukprovider.h"
#include "serializer.h"
#include "deserializer.h"
#include "string-wrapper.h"
#include "libnunchuk/include/nunchuk.h"

using namespace nunchuk;

extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_addSignerToGroup(JNIEnv *env, jobject thiz,
                                                                      jstring group_id,
                                                                      jobject signer,
                                                                      jint index) {
    try {
        auto cSigner = Serializer::convert2CSigner(env, signer);
        auto groupSandbox = NunchukProvider::get()->nu->AddSignerToGroup(
                StringWrapper(env, group_id),
                cSigner,
                index
        );
        return Deserializer::convert2JGroupSandbox(env, groupSandbox);
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return nullptr;
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return nullptr;
    }
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_removeSignerFromGroup(JNIEnv *env, jobject thiz,
                                                                           jstring group_id,
                                                                           jint index) {
    try {
        auto groupSandbox = NunchukProvider::get()->nu->RemoveSignerFromGroup(
                StringWrapper(env, group_id),
                index
        );
        return Deserializer::convert2JGroupSandbox(env, groupSandbox);
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return nullptr;
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return nullptr;
    }
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_finalizeGroup(JNIEnv *env, jobject thiz,
                                                                   jstring group_id,
                                                                   jobject key_set_indexes) {
    try {
        std::set<size_t> c_key_set_indexes;

        jclass setClass = env->FindClass("java/util/Set");
        jmethodID iteratorMethod = env->GetMethodID(setClass, "iterator", "()Ljava/util/Iterator;");

        jclass iteratorClass = env->FindClass("java/util/Iterator");
        jmethodID hasNextMethod = env->GetMethodID(iteratorClass, "hasNext", "()Z");
        jmethodID nextMethod = env->GetMethodID(iteratorClass, "next", "()Ljava/lang/Object;");

        jobject iterator = env->CallObjectMethod(key_set_indexes, iteratorMethod);

        while (env->CallBooleanMethod(iterator, hasNextMethod)) {
            jobject element = env->CallObjectMethod(iterator, nextMethod);
            jint elementInt = env->CallIntMethod(element, env->GetMethodID(env->FindClass("java/lang/Integer"), "intValue", "()I"));
            c_key_set_indexes.insert(static_cast<size_t>(elementInt));
            env->DeleteLocalRef(element);
        }

        env->DeleteLocalRef(iterator);
        env->DeleteLocalRef(setClass);
        env->DeleteLocalRef(iteratorClass);

        auto groupSandbox = NunchukProvider::get()->nu->FinalizeGroup(
                StringWrapper(env, group_id),
                c_key_set_indexes
        );
        return Deserializer::convert2JGroupSandbox(env, groupSandbox);
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return nullptr;
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return nullptr;
    }
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_getGroupWallets(JNIEnv *env, jobject thiz) {
    try {
        auto wallets = NunchukProvider::get()->nu->GetGroupWallets();
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
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_getGroups(JNIEnv *env, jobject thiz) {
    try {
        auto groups = NunchukProvider::get()->nu->GetGroups();
        return Deserializer::convert2JGroupsSandbox(env, groups);
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return env->ExceptionOccurred();
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return env->ExceptionOccurred();
    }
}
extern "C"
JNIEXPORT void JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_enableGroupWallet(JNIEnv *env, jobject thiz,
                                                                       jstring os_name,
                                                                       jstring os_version,
                                                                       jstring app_version,
                                                                       jstring device_class,
                                                                       jstring device_id,
                                                                       jstring access_token) {
    try {
        NunchukProvider::get()->nu->EnableGroupWallet(
                StringWrapper(env, os_name),
                StringWrapper(env, os_version),
                StringWrapper(env, app_version),
                StringWrapper(env, device_class),
                StringWrapper(env, device_id),
                StringWrapper(env, access_token)
        );
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
    }
}
extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_joinGroupWallet(JNIEnv *env, jobject thiz,
                                                                     jstring sandbox_url) {
    try {
        auto groupId = NunchukProvider::get()->nu->ParseGroupUrl(
                StringWrapper(env, sandbox_url)
        ).first;
        auto groupSandbox = NunchukProvider::get()->nu->JoinGroup(groupId);
        return Deserializer::convert2JGroupSandbox(env, groupSandbox);
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return nullptr;
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return nullptr;
    }
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_getGroupWalletMessages(JNIEnv *env,
                                                                            jobject thiz,
                                                                            jstring wallet_id,
                                                                            jint page,
                                                                            jint page_size) {
    try {
        auto messages = NunchukProvider::get()->nu->GetGroupMessages(
                StringWrapper(env, wallet_id),
                page,
                page_size,
                true
        );
        return Deserializer::convert2JGroupMessages(env, messages);
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return nullptr;
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return nullptr;
    }
}
extern "C"
JNIEXPORT void JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_updateGroupWalletConfig(JNIEnv *env,
                                                                             jobject thiz,
                                                                             jstring wallet_id,
                                                                             jint chat_retention_days) {
    try {
        auto setting = NunchukProvider::get()->nu->GetGroupWalletConfig(
                StringWrapper(env, wallet_id)
        );
        setting.set_chat_retention_days(chat_retention_days);
        NunchukProvider::get()->nu->SetGroupWalletConfig(StringWrapper(env, wallet_id), setting);
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
    }
}
extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_getGroupWalletConfig(JNIEnv *env, jobject thiz,
                                                                          jstring wallet_id) {
    try {
        auto setting = NunchukProvider::get()->nu->GetGroupWalletConfig(
                StringWrapper(env, wallet_id)
        );
        return Deserializer::convert2JGroupWalletConfig(env, setting);
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return nullptr;
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return nullptr;
    }
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_getGroupSandbox(JNIEnv *env, jobject thiz,
                                                              jstring group_id) {
    try {
        auto groupSandbox = NunchukProvider::get()->nu->GetGroup(
                StringWrapper(env, group_id)
        );
        return Deserializer::convert2JGroupSandbox(env, groupSandbox);
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return nullptr;
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return nullptr;
    }
}
extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_createGroupSandbox(JNIEnv *env, jobject thiz,
                                                                       jstring name, jint m, jint n,
                                                                       jint address_type) {
    try {
        auto addressType = Serializer::convert2CAddressType(address_type);
        auto wallet = NunchukProvider::get()->nu->CreateGroup(
                StringWrapper(env, name),
                m,
                n,
                addressType
        );
        return Deserializer::convert2JGroupSandbox(env, wallet);
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return nullptr;
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return nullptr;
    }
}


extern "C"
JNIEXPORT jboolean JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_deleteGroupSandbox(JNIEnv *env, jobject thiz,
                                                                        jstring group_id) {
    try {
        NunchukProvider::get()->nu->DeleteGroup(StringWrapper(env, group_id));
        return JNI_TRUE;
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return JNI_FALSE;
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return JNI_FALSE;
    }
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_getGroupOnline(JNIEnv *env, jobject thiz,
                                                                    jstring group_id) {
    try {
        auto online = NunchukProvider::get()->nu->GetGroupOnline(
                StringWrapper(env, group_id)
        );
        return online;
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return 0;
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return 0;
    }
}
extern "C"
JNIEXPORT jstring JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_getGroupDeviceUID(JNIEnv *env, jobject thiz) {
    try {
        return env->NewStringUTF(NunchukProvider::get()->nu->GetGroupDeviceUID().c_str());
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return nullptr;
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return nullptr;
    }
}
extern "C"
JNIEXPORT void JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_startConsumeGroupEvent(JNIEnv *env,
                                                                            jobject thiz) {
    try {
        NunchukProvider::get()->nu->StartConsumeGroupEvent();
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
    }
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_getGroupWalletUnreadMessagesCount(JNIEnv *env,
                                                                                       jobject thiz,
                                                                                       jstring wallet_id) {
    try {
        return NunchukProvider::get()->nu->GetUnreadMessagesCount(
                StringWrapper(env, wallet_id)
        );
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return 0;
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return 0;
    }
}
extern "C"
JNIEXPORT void JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_setGroupWalletLastReadMessage(JNIEnv *env,
                                                                                   jobject thiz,
                                                                                   jstring wallet_id,
                                                                                   jstring message_id) {
    try {
        NunchukProvider::get()->nu->SetLastReadMessage(
                StringWrapper(env, wallet_id),
                StringWrapper(env, message_id)
        );
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
    }
}
extern "C"
JNIEXPORT void JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_stopConsumeGroupEvent(JNIEnv *env,
                                                                           jobject thiz) {
    try {
        if (NunchukProvider::get()->nu == nullptr) {
            return;
        }
        NunchukProvider::get()->nu->StopConsumeGroupEvent();
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
    }
}
extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_joinGroupWalletById(JNIEnv *env, jobject thiz,
                                                                         jstring group_id) {
    try {
        auto groupSandbox = NunchukProvider::get()->nu->JoinGroup(
                StringWrapper(env, group_id)
        );
        return Deserializer::convert2JGroupSandbox(env, groupSandbox);
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return nullptr;
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return nullptr;
    }
}
extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_setSlotOccupied(JNIEnv *env, jobject thiz,
                                                                     jstring group_id, jint index,
                                                                     jboolean value) {
    try {
        auto groupSandbox = NunchukProvider::get()->nu->SetSlotOccupied(
                StringWrapper(env, group_id),
                index,
                value
        );
        return Deserializer::convert2JGroupSandbox(env, groupSandbox);
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return nullptr;
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return nullptr;
    }
}
extern "C"
JNIEXPORT jboolean JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_checkGroupWalletExists(JNIEnv *env,
                                                                            jobject thiz,
                                                                            jstring wallet_id) {
    try {
        auto wallet = NunchukProvider::get()->nu->GetWallet(
                StringWrapper(env, wallet_id)
        );
        return NunchukProvider::get()->nu->CheckGroupWalletExists(wallet);
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
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_sendGroupWalletMessage(JNIEnv *env,
                                                                            jobject thiz,
                                                                            jstring wallet_id,
                                                                            jstring message,
                                                                            jobject single_signer) {
    try {
        if (single_signer == nullptr) {
            NunchukProvider::get()->nu->SendGroupMessage(
                    StringWrapper(env, wallet_id),
                    StringWrapper(env, message)
            );
        } else {
            NunchukProvider::get()->nu->SendGroupMessage(
                    StringWrapper(env, wallet_id),
                    StringWrapper(env, message),
                    Serializer::convert2CSigner(env, single_signer)
            );
        }
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
    }
}