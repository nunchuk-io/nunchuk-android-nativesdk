#include <cstring>
#include <jni.h>
#include <nunchuk.h>
#include <nunchukmatrix.h>
#include "nunchukprovider.h"
#include "serializer.h"
#include "deserializer.h"
#include "string-wrapper.h"

using namespace nunchuk;

extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_initSharedWallet(
        JNIEnv *env,
        jobject thiz,
        jstring room_id,
        jstring name,
        jint require_signs,
        jint total_signs,
        jint address_type,
        jboolean is_escrow,
        jstring des,
        jobject signers
) {
    syslog(LOG_DEBUG, "[JNI]initSharedWallet()");
    try {
        AddressType type = Serializer::convert2CAddressType(address_type);
        StringWrapper roomIdStr(env, room_id);
        StringWrapper nameStr(env, name);
        StringWrapper desStr(env, des);
        syslog(LOG_DEBUG, "[JNI]roomId::%s", std::string(roomIdStr).c_str());
        syslog(LOG_DEBUG, "[JNI]name::%s", std::string(nameStr).c_str());
        syslog(LOG_DEBUG, "[JNI]m::%d", require_signs);
        syslog(LOG_DEBUG, "[JNI]n::%d", total_signs);
        syslog(LOG_DEBUG, "[JNI]address_type::%d", address_type);
        syslog(LOG_DEBUG, "[JNI]is_escrow::%d", is_escrow);
        auto result = NunchukProvider::get()->nuMatrix->InitWallet(
                roomIdStr,
                nameStr,
                require_signs,
                total_signs,
                type,
                is_escrow,
                desStr,
                Serializer::convert2CSigners(env, signers)
        );
        return Deserializer::convert2JMatrixEvent(env, result);
    } catch (BaseException &e) {
        syslog(LOG_DEBUG, "[JNI] initSharedWallet error::%s", e.what());
        Deserializer::convert2JException(env, e);
        return env->ExceptionOccurred();
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return env->ExceptionOccurred();
    }
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_joinSharedWallet(
        JNIEnv *env,
        jobject thiz,
        jstring room_id,
        jobject signer
) {
    syslog(LOG_DEBUG, "[JNI]joinSharedWallet()");
    try {
        StringWrapper roomIdStr(env, room_id);
        syslog(LOG_DEBUG, "[JNI]roomId::%s", std::string(roomIdStr).c_str());
        SingleSigner singleSigner = Serializer::convert2CSigner(env, signer);
        syslog(LOG_DEBUG, "[JNI]masterSignerId::%s", singleSigner.get_master_signer_id().c_str());
        auto result = NunchukProvider::get()->nuMatrix->JoinWallet(
                roomIdStr,
                singleSigner
        );
        return Deserializer::convert2JMatrixEvent(env, result);
    } catch (BaseException &e) {
        syslog(LOG_DEBUG, "[JNI] joinSharedWallet error::%s", e.what());
        Deserializer::convert2JException(env, e);
        return env->ExceptionOccurred();
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return env->ExceptionOccurred();
    }
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_leaveSharedWallet(
        JNIEnv *env,
        jobject thiz,
        jstring room_id,
        jstring join_event_id,
        jstring reason
) {
    syslog(LOG_DEBUG, "[JNI]leaveSharedWallet()");
    try {
        StringWrapper roomIdStr(env, room_id);
        StringWrapper joinEventIdStr(env, join_event_id);
        StringWrapper reasonStr(env, reason);
        syslog(LOG_DEBUG, "[JNI]roomId::%s", std::string(roomIdStr).c_str());
        syslog(LOG_DEBUG, "[JNI]join_event_id::%s", std::string(joinEventIdStr).c_str());
        syslog(LOG_DEBUG, "[JNI]reason::%s", std::string(reasonStr).c_str());
        auto result = NunchukProvider::get()->nuMatrix->LeaveWallet(
                roomIdStr,
                joinEventIdStr,
                reasonStr
        );
        return Deserializer::convert2JMatrixEvent(env, result);
    } catch (BaseException &e) {
        syslog(LOG_DEBUG, "[JNI] leaveSharedWallet error::%s", e.what());
        Deserializer::convert2JException(env, e);
        return env->ExceptionOccurred();
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return env->ExceptionOccurred();
    }
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_cancelSharedWallet(
        JNIEnv *env,
        jobject thiz,
        jstring room_id,
        jstring reason
) {
    syslog(LOG_DEBUG, "[JNI]cancelSharedWallet()");
    try {
        StringWrapper roomIdStr(env, room_id);
        StringWrapper reasonStr(env, reason);
        syslog(LOG_DEBUG, "[JNI]roomId::%s", std::string(roomIdStr).c_str());
        syslog(LOG_DEBUG, "[JNI]reason::%s", std::string(reasonStr).c_str());
        auto result = NunchukProvider::get()->nuMatrix->CancelWallet(
                roomIdStr,
                reasonStr
        );
        return Deserializer::convert2JMatrixEvent(env, result);
    } catch (BaseException &e) {
        syslog(LOG_DEBUG, "[JNI] cancelSharedWallet error::%s", e.what());
        Deserializer::convert2JException(env, e);
        return env->ExceptionOccurred();
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return env->ExceptionOccurred();
    }
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_createSharedWallet(
        JNIEnv *env,
        jobject thiz,
        jstring room_id
) {
    syslog(LOG_DEBUG, "[JNI]createSharedWallet()");
    try {
        StringWrapper roomIdStr(env, room_id);
        syslog(LOG_DEBUG, "[JNI]roomId::%s", std::string(roomIdStr).c_str());
        auto result = NunchukProvider::get()->nuMatrix->CreateWallet(
                NunchukProvider::get()->nu,
                roomIdStr
        );
        return Deserializer::convert2JMatrixEvent(env, result);
    } catch (BaseException &e) {
        syslog(LOG_DEBUG, "[JNI] createSharedWallet error::%s", e.what());
        Deserializer::convert2JException(env, e);
        return env->ExceptionOccurred();
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return env->ExceptionOccurred();
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_consumeEvent(
        JNIEnv *env,
        jobject thiz,
        jobject event
) {
    syslog(LOG_DEBUG, "[JNI]consumeEvent()");
    try {
        auto matrixEvent = Serializer::convert2CMatrixEvent(env, event);
        NunchukProvider::get()->nuMatrix->ConsumeEvent(
                NunchukProvider::get()->nu,
                matrixEvent
        );
    } catch (BaseException &e) {
        syslog(LOG_DEBUG, "[JNI] consumeEvent error::%s", e.what());
        Deserializer::convert2JException(env, e);
        env->ExceptionOccurred();
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        env->ExceptionOccurred();
    }
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_hasRoomWallet(
        JNIEnv *env,
        jobject thiz,
        jstring room_id
) {
    try {
        return NunchukProvider::get()->nuMatrix->HasRoomWallet(
                StringWrapper(env, room_id)
        );
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return JNI_FALSE;
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return JNI_FALSE;
    }
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_getRoomWallet(
        JNIEnv *env,
        jobject thiz,
        jstring room_id
) {
    syslog(LOG_DEBUG, "[JNI]getRoomWallet()");
    StringWrapper roomIdStr(env, room_id);
    syslog(LOG_DEBUG, "[JNI]roomId::%s", std::string(roomIdStr).c_str());
    try {
        auto roomWallet = NunchukProvider::get()->nuMatrix->GetRoomWallet(
                roomIdStr
        );
        return Deserializer::convert2JRoomWallet(env, roomWallet);
    } catch (BaseException &e) {
        syslog(LOG_DEBUG, "[JNI] getRoomWallet error::%s", e.what());
        Deserializer::convert2JException(env, e);
        return env->ExceptionOccurred();
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return env->ExceptionOccurred();
    }
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_getAllRoomWallets(
        JNIEnv *env,
        jobject thiz
) {
    try {
        auto roomWallets = NunchukProvider::get()->nuMatrix->GetAllRoomWallets();
        return Deserializer::convert2JRoomWallets(env, roomWallets);
    } catch (BaseException &e) {
        syslog(LOG_DEBUG, "[JNI] getAllRoomWallets error::%s", e.what());
        Deserializer::convert2JException(env, e);
        return env->ExceptionOccurred();
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return env->ExceptionOccurred();
    }
}