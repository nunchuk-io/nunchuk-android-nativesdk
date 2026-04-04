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
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_initRoomTransaction(
        JNIEnv *env,
        jobject thiz,
        jstring room_id,
        jobject outputs,
        jstring memo,
        jobject inputs,
        jobject fee_rate,
        jboolean subtract_fee_from_amount
) {
    syslog(LOG_DEBUG, "[JNI]initRoomTransaction()");
    try {
        auto result = NunchukProvider::get()->nuMatrix->InitTransaction(
                NunchukProvider::get()->nu,
                StringWrapper(env, room_id),
                Serializer::convert2CAmountsMap(env, outputs),
                StringWrapper(env, memo),
                Serializer::convert2CUnspentOutputs(env, inputs),
                Serializer::convert2CAmount(env, fee_rate),
                subtract_fee_from_amount
        );
        return Deserializer::convert2JMatrixEvent(env, result);
    } catch (BaseException &e) {
        syslog(LOG_DEBUG, "[JNI] initRoomTransaction error::%s", e.what());
        Deserializer::convert2JException(env, e);
        return env->ExceptionOccurred();
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return env->ExceptionOccurred();
    }
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_signRoomTransaction(
        JNIEnv *env,
        jobject thiz,
        jstring init_event_id,
        jobject device
) {
    syslog(LOG_DEBUG, "[JNI]signRoomTransaction()");
    try {
        auto result = NunchukProvider::get()->nuMatrix->SignTransaction(
                NunchukProvider::get()->nu,
                StringWrapper(env, init_event_id),
                Serializer::convert2CDevice(env, device)
        );
        return Deserializer::convert2JMatrixEvent(env, result);
    } catch (BaseException &e) {
        syslog(LOG_DEBUG, "[JNI] signRoomTransaction error::%s", e.what());
        Deserializer::convert2JException(env, e);
        return env->ExceptionOccurred();
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return env->ExceptionOccurred();
    }
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_rejectRoomTransaction(
        JNIEnv *env,
        jobject thiz,
        jstring init_event_id,
        jstring reason
) {
    syslog(LOG_DEBUG, "[JNI]rejectRoomTransaction()");
    try {
        auto result = NunchukProvider::get()->nuMatrix->RejectTransaction(
                StringWrapper(env, init_event_id),
                StringWrapper(env, reason)
        );
        return Deserializer::convert2JMatrixEvent(env, result);
    } catch (BaseException &e) {
        syslog(LOG_DEBUG, "[JNI] rejectRoomTransaction error::%s", e.what());
        Deserializer::convert2JException(env, e);
        return env->ExceptionOccurred();
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return env->ExceptionOccurred();
    }
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_cancelRoomTransaction(
        JNIEnv *env,
        jobject thiz,
        jstring init_event_id,
        jstring reason
) {
    syslog(LOG_DEBUG, "[JNI]cancelRoomTransaction()");
    try {
        auto result = NunchukProvider::get()->nuMatrix->CancelTransaction(
                StringWrapper(env, init_event_id),
                StringWrapper(env, reason)
        );
        return Deserializer::convert2JMatrixEvent(env, result);
    } catch (BaseException &e) {
        syslog(LOG_DEBUG, "[JNI] cancelRoomTransaction error::%s", e.what());
        Deserializer::convert2JException(env, e);
        return env->ExceptionOccurred();
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return env->ExceptionOccurred();
    }
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_broadcastRoomTransaction(
        JNIEnv *env,
        jobject thiz,
        jstring init_event_id
) {
    syslog(LOG_DEBUG, "[JNI]broadcastRoomTransaction()");
    try {
        auto result = NunchukProvider::get()->nuMatrix->BroadcastTransaction(
                NunchukProvider::get()->nu,
                StringWrapper(env, init_event_id)
        );
        return Deserializer::convert2JMatrixEvent(env, result);
    } catch (BaseException &e) {
        syslog(LOG_DEBUG, "[JNI] broadcastRoomTransaction error::%s", e.what());
        Deserializer::convert2JException(env, e);
        return env->ExceptionOccurred();
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return env->ExceptionOccurred();
    }
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_getRoomTransaction(
        JNIEnv *env,
        jobject thiz,
        jstring init_event_id
) {
    syslog(LOG_DEBUG, "[JNI]getRoomTransaction()");
    StringWrapper initEventIdStr(env, init_event_id);
    syslog(LOG_DEBUG, "[JNI]init_event_id::%s", std::string(initEventIdStr).c_str());
    try {
        auto result = NunchukProvider::get()->nuMatrix->GetRoomTransaction(
                initEventIdStr
        );
        return Deserializer::convert2JRoomTransaction(env, result);
    } catch (BaseException &e) {
        syslog(LOG_DEBUG, "[JNI] getRoomTransaction error::%s", e.what());
        Deserializer::convert2JException(env, e);
        return env->ExceptionOccurred();
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return env->ExceptionOccurred();
    }
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_getPendingTransactions(
        JNIEnv *env,
        jobject thiz,
        jstring room_id
) {
    syslog(LOG_DEBUG, "[JNI]getPendingTransactions()");
    StringWrapper roomIdStr(env, room_id);
    syslog(LOG_DEBUG, "[JNI]room_id::%s", std::string(roomIdStr).c_str());
    try {
        auto result = NunchukProvider::get()->nuMatrix->GetPendingTransactions(
                roomIdStr
        );
        return Deserializer::convert2JRoomTransactions(env, result);
    } catch (BaseException &e) {
        syslog(LOG_DEBUG, "[JNI] getPendingTransactions error::%s", e.what());
        Deserializer::convert2JException(env, e);
        return env->ExceptionOccurred();
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return env->ExceptionOccurred();
    }
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_getTransactionId(
        JNIEnv *env,
        jobject thiz,
        jstring event_id
) {
    syslog(LOG_DEBUG, "[JNI]getTransactionId()");
    StringWrapper eventIdStr(env, event_id);
    syslog(LOG_DEBUG, "[JNI]event_id::%s", std::string(eventIdStr).c_str());
    try {
        auto result = NunchukProvider::get()->nuMatrix->GetTransactionId(
                eventIdStr
        );
        return env->NewStringUTF(result.c_str());
    } catch (BaseException &e) {
        syslog(LOG_DEBUG, "[JNI] getTransactionId error::%s", e.what());
        return env->NewStringUTF("");
    } catch (std::exception &e) {
        syslog(LOG_DEBUG, "[JNI] getTransactionId error::%s", e.what());
        return env->NewStringUTF("");
    }
}