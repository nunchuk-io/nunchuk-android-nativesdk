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