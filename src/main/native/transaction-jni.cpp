#include <cstring>
#include <jni.h>
#include <nunchuk.h>
#include "nunchukprovider.h"
#include "serializer.h"
#include "deserializer.h"
#include "modelprovider.h"

using namespace nunchuk;

extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_broadcastTransaction(
        JNIEnv *env,
        jobject thiz,
        jstring wallet_id,
        jstring tx_id
) {
    try {
        auto transaction = NunchukProvider::get()->nu->BroadcastTransaction(
            env->GetStringUTFChars(wallet_id, JNI_FALSE),
            env->GetStringUTFChars(tx_id, JNI_FALSE)
        );
        return Deserializer::convert2JTransaction(env, transaction);
    } catch (std::exception &e) {
        Deserializer::convert2JException(env, e.what());
        env->ExceptionOccurred();
        return ModelProvider::createEmptyTransaction(env);
    }
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_createTransaction(
        JNIEnv *env,
        jobject thiz,
        jstring wallet_id,
        jobject outputs,
        jstring memo,
        jobject inputs,
        jobject fee_rate,
        jboolean subtract_fee_from_amount
) {
    syslog(LOG_DEBUG, "[JNI]createTransaction()");
    try {
        auto transaction = NunchukProvider::get()->nu->CreateTransaction(
                env->GetStringUTFChars(wallet_id, JNI_FALSE),
                Serializer::convert2CAmountsMap(env, outputs),
                env->GetStringUTFChars(memo, JNI_FALSE),
                Serializer::convert2CUnspentOutputs(env, inputs),
                Serializer::convert2CAmount(env, fee_rate),
                subtract_fee_from_amount
        );
        return Deserializer::convert2JTransaction(env, transaction);
    } catch (std::exception &e) {
        Deserializer::convert2JException(env, e.what());
        env->ExceptionOccurred();
        return ModelProvider::createEmptyTransaction(env);
    }
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_draftTransaction(
        JNIEnv *env,
        jobject thiz,
        jstring wallet_id,
        jobject outputs,
        jobject inputs,
        jobject fee_rate,
        jboolean subtract_fee_from_amount
) {
    try {
        auto transaction = NunchukProvider::get()->nu->DraftTransaction(
                env->GetStringUTFChars(wallet_id, JNI_FALSE),
                Serializer::convert2CAmountsMap(env, outputs),
                Serializer::convert2CUnspentOutputs(env, inputs),
                Serializer::convert2CAmount(env, fee_rate),
                subtract_fee_from_amount
        );
        return Deserializer::convert2JTransaction(env, transaction);
    } catch (std::exception &e) {
        Deserializer::convert2JException(env, e.what());
        env->ExceptionOccurred();
        return ModelProvider::createEmptyTransaction(env);
    }
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_deleteTransaction(
        JNIEnv *env,
        jobject thiz,
        jstring wallet_id,
        jstring tx_id
) {
    try {
        return NunchukProvider::get()->nu->DeleteTransaction(
                env->GetStringUTFChars(wallet_id, JNI_FALSE),
                env->GetStringUTFChars(tx_id, JNI_FALSE)
        );
    } catch (std::exception &e) {
        Deserializer::convert2JException(env, e.what());
        env->ExceptionOccurred();
        return JNI_FALSE;
    }
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_exportCoboTransaction(
        JNIEnv *env,
        jobject thiz,
        jstring wallet_id,
        jstring tx_id
) {
    try {
        auto values = NunchukProvider::get()->nu->ExportCoboTransaction(
                env->GetStringUTFChars(wallet_id, JNI_FALSE),
                env->GetStringUTFChars(tx_id, JNI_FALSE)
        );
        return Deserializer::convert2JListString(env, values);
    } catch (std::exception &e) {
        Deserializer::convert2JException(env, e.what());
        env->ExceptionOccurred();
        return ModelProvider::createEmptyList(env);
    }
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_exportTransaction(
        JNIEnv *env,
        jobject thiz,
        jstring wallet_id,
        jstring tx_id,
        jstring file_path
) {
    try {
        return NunchukProvider::get()->nu->ExportTransaction(
                env->GetStringUTFChars(wallet_id, JNI_FALSE),
                env->GetStringUTFChars(tx_id, JNI_FALSE),
                env->GetStringUTFChars(file_path, JNI_FALSE)
        );
    } catch (std::exception &e) {
        Deserializer::convert2JException(env, e.what());
        env->ExceptionOccurred();
        return JNI_FALSE;
    }
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_getTransaction(
        JNIEnv *env,
        jobject thiz,
        jstring wallet_id,
        jstring tx_id
) {
    try {
        auto transaction = NunchukProvider::get()->nu->GetTransaction(
                env->GetStringUTFChars(wallet_id, JNI_FALSE),
                env->GetStringUTFChars(tx_id, JNI_FALSE)
        );
        return Deserializer::convert2JTransaction(env, transaction);
    } catch (std::exception &e) {
        Deserializer::convert2JException(env, e.what());
        env->ExceptionOccurred();
        return ModelProvider::createEmptyTransaction(env);
    }
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_replaceTransaction(
        JNIEnv *env,
        jobject thiz,
        jstring wallet_id,
        jstring tx_id,
        jobject new_fee_rate
) {
    try {
        auto transaction = NunchukProvider::get()->nu->ReplaceTransaction(
                env->GetStringUTFChars(wallet_id, JNI_FALSE),
                env->GetStringUTFChars(tx_id, JNI_FALSE),
                Serializer::convert2CAmount(env, new_fee_rate)
        );
        return Deserializer::convert2JTransaction(env, transaction);
    } catch (std::exception &e) {
        Deserializer::convert2JException(env, e.what());
        env->ExceptionOccurred();
        return ModelProvider::createEmptyTransaction(env);
    }
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_importTransaction(
        JNIEnv *env,
        jobject thiz,
        jstring wallet_id,
        jstring file_path
) {
    try {
        auto transaction = NunchukProvider::get()->nu->ImportTransaction(
                env->GetStringUTFChars(wallet_id, JNI_FALSE),
                env->GetStringUTFChars(file_path, JNI_FALSE)
        );
        return Deserializer::convert2JTransaction(env, transaction);
    } catch (std::exception &e) {
        Deserializer::convert2JException(env, e.what());
        env->ExceptionOccurred();
        return ModelProvider::createEmptyTransaction(env);
    }
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_getTransactionHistory(
        JNIEnv *env,
        jobject thiz,
        jstring wallet_id,
        jint count,
        jint skip
) {
    try {
        auto transactions = NunchukProvider::get()->nu->GetTransactionHistory(
                env->GetStringUTFChars(wallet_id, JNI_FALSE),
                count,
                skip
        );
        return Deserializer::convert2JTransactions(env, transactions);
    } catch (std::exception &e) {
        Deserializer::convert2JException(env, e.what());
        env->ExceptionOccurred();
        return ModelProvider::createEmptyList(env);
    }
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_importCoboTransaction(
        JNIEnv *env,
        jobject thiz,
        jstring wallet_id,
        jobject qr_data
) {
    // TODO
    return ModelProvider::createEmptyTransaction(env);
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_updateTransactionMemo(
        JNIEnv *env,
        jobject thiz,
        jstring wallet_id,
        jstring tx_id,
        jstring new_memo
) {
    try {
        return NunchukProvider::get()->nu->UpdateTransactionMemo(
                env->GetStringUTFChars(wallet_id, JNI_FALSE),
                env->GetStringUTFChars(tx_id, JNI_FALSE),
                env->GetStringUTFChars(new_memo, JNI_FALSE)
        );
    } catch (std::exception &e) {
        Deserializer::convert2JException(env, e.what());
        env->ExceptionOccurred();
        return JNI_FALSE;
    }
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_signTransaction(
        JNIEnv *env,
        jobject thiz,
        jstring wallet_id,
        jstring tx_id,
        jobject device
) {
    try {
        auto transaction =  NunchukProvider::get()->nu->SignTransaction(
                env->GetStringUTFChars(wallet_id, JNI_FALSE),
                env->GetStringUTFChars(tx_id, JNI_FALSE),
                Serializer::convert2CDevice(env, device)
        );
        return Deserializer::convert2JTransaction(env, transaction);
    } catch (std::exception &e) {
        Deserializer::convert2JException(env, e.what());
        env->ExceptionOccurred();
        return ModelProvider::createEmptyTransaction(env);
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_exportTransactionHistory(
        JNIEnv *env,
        jobject thiz,
        jstring wallet_id,
        jstring file_path,
        jint format
) {
    try {
        NunchukProvider::get()->nu->ExportTransactionHistory(
            env->GetStringUTFChars(wallet_id, JNI_FALSE),
            env->GetStringUTFChars(file_path, JNI_FALSE),
            Serializer::convert2CExportFormat(format)
        );
    } catch (std::exception &e) {
        Deserializer::convert2JException(env, e.what());
        env->ExceptionOccurred();
    }
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_getAddresses(
        JNIEnv *env,
        jobject thiz,
        jstring wallet_id,
        jboolean used,
        jboolean internal
) {
    try {
        auto values = NunchukProvider::get()->nu->GetAddresses(
            env->GetStringUTFChars(wallet_id, JNI_FALSE),
            used,
            internal
        );
        return Deserializer::convert2JListString(env, values);
    } catch (std::exception &e) {
        Deserializer::convert2JException(env, e.what());
        env->ExceptionOccurred();
        return ModelProvider::createEmptyList(env);
    }
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_getAddressBalance(
        JNIEnv *env,
        jobject thiz,
        jstring wallet_id,
        jstring address
) {
    try {
        auto value = NunchukProvider::get()->nu->GetAddressBalance(
                env->GetStringUTFChars(wallet_id, JNI_FALSE),
                env->GetStringUTFChars(address, JNI_FALSE));
        return Deserializer::convert2JAmount(env, value);
    } catch (std::exception &e) {
        Deserializer::convert2JException(env, e.what());
        env->ExceptionOccurred();
        return ModelProvider::createEmptyAmount(env);
    }
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_getUnspentOutputs(
        JNIEnv *env,
        jobject thiz,
        jstring wallet_id
) {
    // TODO: implement getUnspentOutputs()
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_newAddress(
        JNIEnv *env,
        jobject thiz,
        jstring wallet_id,
        jboolean internal
) {
    try {
        auto value = NunchukProvider::get()->nu->NewAddress(
                env->GetStringUTFChars(wallet_id, JNI_FALSE),
                internal);
        return env->NewStringUTF(value.c_str());
    } catch (std::exception &e) {
        Deserializer::convert2JException(env, e.what());
        env->ExceptionOccurred();
        return env->NewStringUTF("");
    }
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_estimateFee(
        JNIEnv *env,
        jobject thiz,
        jint type
) {
    try {
        auto value = NunchukProvider::get()->nu->EstimateFee(type);
        return Deserializer::convert2JAmount(env, value);
    } catch (std::exception &e) {
        Deserializer::convert2JException(env, e.what());
        env->ExceptionOccurred();
        return ModelProvider::createEmptyAmount(env);
    }

}