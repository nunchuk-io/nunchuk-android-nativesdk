#include <cstring>
#include <jni.h>
#include <nunchuk.h>
#include <nunchukmatrix.h>
#include "nunchukprovider.h"
#include "serializer.h"
#include "deserializer.h"

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
                env->GetStringUTFChars(room_id, JNI_FALSE),
                Serializer::convert2CAmountsMap(env, outputs),
                env->GetStringUTFChars(memo, JNI_FALSE),
                Serializer::convert2CUnspentOutputs(env, inputs),
                Serializer::convert2CAmount(env, fee_rate),
                subtract_fee_from_amount
        );
        return Deserializer::convert2JMatrixEvent(env, result);
    } catch (std::exception &e) {
        syslog(LOG_DEBUG, "[JNI] initRoomTransaction error::%s", e.what());
        Deserializer::convert2JException(env, e.what());
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
                env->GetStringUTFChars(init_event_id, JNI_FALSE),
                Serializer::convert2CDevice(env, device)
        );
        return Deserializer::convert2JMatrixEvent(env, result);
    } catch (std::exception &e) {
        syslog(LOG_DEBUG, "[JNI] signRoomTransaction error::%s", e.what());
        Deserializer::convert2JException(env, e.what());
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
                env->GetStringUTFChars(init_event_id, JNI_FALSE),
                env->GetStringUTFChars(reason, JNI_FALSE)
        );
        return Deserializer::convert2JMatrixEvent(env, result);
    } catch (std::exception &e) {
        syslog(LOG_DEBUG, "[JNI] rejectRoomTransaction error::%s", e.what());
        Deserializer::convert2JException(env, e.what());
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
                env->GetStringUTFChars(init_event_id, JNI_FALSE),
                env->GetStringUTFChars(reason, JNI_FALSE)
        );
        return Deserializer::convert2JMatrixEvent(env, result);
    } catch (std::exception &e) {
        syslog(LOG_DEBUG, "[JNI] cancelRoomTransaction error::%s", e.what());
        Deserializer::convert2JException(env, e.what());
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
                env->GetStringUTFChars(init_event_id, JNI_FALSE)
        );
        return Deserializer::convert2JMatrixEvent(env, result);
    } catch (std::exception &e) {
        syslog(LOG_DEBUG, "[JNI] broadcastRoomTransaction error::%s", e.what());
        Deserializer::convert2JException(env, e.what());
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
    syslog(LOG_DEBUG, "[JNI]init_event_id::%s", env->GetStringUTFChars(init_event_id, JNI_FALSE));
    try {
        auto result = NunchukProvider::get()->nuMatrix->GetRoomTransaction(
                env->GetStringUTFChars(init_event_id, JNI_FALSE)
        );
        return Deserializer::convert2JRoomTransaction(env, result);
    } catch (std::exception &e) {
        syslog(LOG_DEBUG, "[JNI] getRoomTransaction error::%s", e.what());
        Deserializer::convert2JException(env, e.what());
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
    syslog(LOG_DEBUG, "[JNI]room_id::%s", env->GetStringUTFChars(room_id, JNI_FALSE));
    try {
        auto result = NunchukProvider::get()->nuMatrix->GetPendingTransactions(
                env->GetStringUTFChars(room_id, JNI_FALSE)
        );
        return Deserializer::convert2JRoomTransactions(env, result);
    } catch (std::exception &e) {
        syslog(LOG_DEBUG, "[JNI] getPendingTransactions error::%s", e.what());
        Deserializer::convert2JException(env, e.what());
        return env->ExceptionOccurred();
    }
}