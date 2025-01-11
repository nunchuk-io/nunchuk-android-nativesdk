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
                                                                      jobject signer) {
    try {
        auto cSigner = Serializer::convert2CSigner(env, signer);
        auto groupSandbox = NunchukProvider::get()->nu->AddSignerToGroup(
                StringWrapper(env, group_id),
                cSigner
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
                                                                           jobject signer) {
    try {
        auto cSigner = Serializer::convert2CSigner(env, signer);
        auto groupSandbox = NunchukProvider::get()->nu->RemoveSignerFromGroup(
                StringWrapper(env, group_id),
                cSigner
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
                                                                   jstring group_id) {
    try {
        auto groupSandbox = NunchukProvider::get()->nu->FinalizeGroup(
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
JNIEXPORT void JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_sendGroupWalletMessage(JNIEnv *env,
                                                                            jobject thiz,
                                                                            jstring wallet_id,
                                                                            jstring message,
                                                                            jobject single_signer) {
    try {
        auto cSigner = Serializer::convert2CSigner(env, single_signer);
        NunchukProvider::get()->nu->SendGroupMessage(
                StringWrapper(env, wallet_id),
                StringWrapper(env, message),
                cSigner
        );
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
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

// virtual GroupSandbox GetGroup(const std::string& groupId) = 0;
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