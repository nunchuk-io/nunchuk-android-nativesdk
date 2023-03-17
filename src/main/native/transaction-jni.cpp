#include <cstring>
#include <jni.h>
#include <nunchuk.h>
#include "nunchukprovider.h"
#include "serializer.h"
#include "deserializer.h"
#include "modelprovider.h"
#include "string-wrapper.h"

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
        auto txInputs = Serializer::convert2CTxInputs(env, inputs);
        auto txInputUnspentOutputs = NunchukProvider::get()->nu->GetUnspentOutputsFromTxInputs(
                StringWrapper(env, wallet_id),
                txInputs);
        auto transaction = NunchukProvider::get()->nu->DraftTransaction(
                env->GetStringUTFChars(wallet_id, JNI_FALSE),
                Serializer::convert2CAmountsMap(env, outputs),
                txInputUnspentOutputs,
                Serializer::convert2CAmount(env, fee_rate),
                subtract_fee_from_amount
        );
        return Deserializer::convert2JTransaction(env, transaction);
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return env->ExceptionOccurred();
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return env->ExceptionOccurred();
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
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        env->ExceptionOccurred();
        return JNI_FALSE;
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        env->ExceptionOccurred();
        return JNI_FALSE;
    }
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_exportKeystoneTransaction(
        JNIEnv *env,
        jobject thiz,
        jstring wallet_id,
        jstring tx_id,
        jint density
) {
    try {
        auto transaction = NunchukProvider::get()->nu->ExportKeystoneTransaction(
                env->GetStringUTFChars(wallet_id, JNI_FALSE),
                env->GetStringUTFChars(tx_id, JNI_FALSE),
                density
        );
        return Deserializer::convert2JListString(env, transaction);
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return env->ExceptionOccurred();
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return env->ExceptionOccurred();
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
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        env->ExceptionOccurred();
        return JNI_FALSE;
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
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
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_importKeystoneTransaction(
        JNIEnv *env,
        jobject thiz,
        jstring wallet_id,
        jobject qr_data
) {
    try {
        auto values = NunchukProvider::get()->nu->ImportKeystoneTransaction(
                env->GetStringUTFChars(wallet_id, JNI_FALSE),
                Serializer::convert2CListString(env, qr_data)
        );
        return Deserializer::convert2JTransaction(env, values);
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return env->ExceptionOccurred();
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return env->ExceptionOccurred();
    }
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
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        env->ExceptionOccurred();
        return JNI_FALSE;
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
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
        auto transaction = NunchukProvider::get()->nu->SignTransaction(
                env->GetStringUTFChars(wallet_id, JNI_FALSE),
                env->GetStringUTFChars(tx_id, JNI_FALSE),
                Serializer::convert2CDevice(env, device)
        );
        return Deserializer::convert2JTransaction(env, transaction);
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
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        env->ExceptionOccurred();
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
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
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_getUnspentOutputs(
        JNIEnv *env,
        jobject thiz,
        jstring wallet_id
) {
    try {
        auto value = NunchukProvider::get()->nu->GetUnspentOutputs(StringWrapper(env, wallet_id));
        return Deserializer::convert2JUnspentOutputs(env, value);
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return env->ExceptionOccurred();
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return env->ExceptionOccurred();
    }
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
    } catch (BaseException &e) {
        return env->NewStringUTF("");
    } catch (std::exception &e) {
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
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_loadTransaction(JNIEnv *env, jobject thiz,
                                                                     jstring tx_id) {
    try {
        auto transaction = NunchukProvider::get()->nu->FetchTransaction(
                StringWrapper(env, tx_id)
        );
        return Deserializer::convert2JTransaction(env, transaction);
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
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_signAirgapTransaction(JNIEnv *env,
                                                                           jobject thiz,
                                                                           jstring init_event_id,
                                                                           jstring master_signer_id) {
    try {
        auto value = NunchukProvider::get()->nuMatrix->SignAirgapTransaction(
                NunchukProvider::get()->nu,
                StringWrapper(env, init_event_id),
                StringWrapper(env, master_signer_id)
        );
        return Deserializer::convert2JMatrixEvent(env, value);
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
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_parseBtcUri(JNIEnv *env, jobject thiz,
                                                                 jstring uri) {
    try {
        auto value = Utils::ParseBtcUri(StringWrapper(env, uri));
        return Deserializer::convert2JBtcUri(env, value);
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
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_importPsbt(JNIEnv *env, jobject thiz,
                                                                jstring wallet_id, jstring psbt) {
    try {
        auto transaction = NunchukProvider::get()->nu->ImportPsbt(
                StringWrapper(env, wallet_id),
                StringWrapper(env, psbt)
        );
        return Deserializer::convert2JTransaction(env, transaction);
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
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_updateTransaction(JNIEnv *env, jobject thiz,
                                                                       jstring wallet_id,
                                                                       jstring tx_id,
                                                                       jstring new_tx_id,
                                                                       jstring raw_tx,
                                                                       jstring reject_msg) {
    try {
        auto tx = NunchukProvider::get()->nu->UpdateTransaction(
                StringWrapper(env, wallet_id),
                StringWrapper(env, tx_id),
                StringWrapper(env, new_tx_id),
                StringWrapper(env, raw_tx),
                StringWrapper(env, reject_msg)
        );
        return Deserializer::convert2JTransaction(env, tx);
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
    }
}
extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_createTransactionWallet(JNIEnv *env,
                                                                             jobject thiz,
                                                                             jobject signer,
                                                                             jstring psbt,
                                                                             jstring sub_amount,
                                                                             jstring fee_rate,
                                                                             jstring fee) {
    try {
        Wallet dummy_wallet = Wallet(false);
        SingleSigner singleSigner = Serializer::convert2CSigner(env, signer);
        dummy_wallet.set_signers({singleSigner});
        Transaction tx = Utils::DecodeTx(dummy_wallet, StringWrapper(env, psbt),
                                         Utils::AmountFromValue(StringWrapper(env, sub_amount)),
                                         Utils::AmountFromValue(StringWrapper(env, fee)),
                                         Utils::AmountFromValue(StringWrapper(env, fee_rate)));
        auto signed_tx = NunchukProvider::get()->nu->SignTransaction(dummy_wallet, tx,
                                                                     Device(singleSigner.get_master_fingerprint()));
        return Deserializer::convert2JTransaction(env, signed_tx);
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
    }
    return nullptr;
}
extern "C"
JNIEXPORT void JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_updateTransactionSchedule(JNIEnv *env,
                                                                               jobject thiz,
                                                                               jstring wallet_id,
                                                                               jstring tx_id,
                                                                               jlong broadcast_time) {
    try {
        NunchukProvider::get()->nu->UpdateTransactionSchedule(
                StringWrapper(env, wallet_id),
                StringWrapper(env, tx_id),
                broadcast_time);
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
    }
}
extern "C"
JNIEXPORT jboolean JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_lockCoin(JNIEnv *env, jobject thiz,
                                                              jstring wallet_id, jstring tx_id,
                                                              jint vout) {
    try {
        return NunchukProvider::get()->nu->LockCoin(
                StringWrapper(env, wallet_id),
                StringWrapper(env, tx_id),
                vout);
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
    }
    return false;
}
extern "C"
JNIEXPORT jboolean JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_unlockCoin(JNIEnv *env, jobject thiz,
                                                                jstring wallet_id,
                                                                jstring tx_id, jint vout) {
    try {
        return NunchukProvider::get()->nu->UnlockCoin(
                StringWrapper(env, wallet_id),
                StringWrapper(env, tx_id),
                vout);
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
    }
    return false;
}
extern "C"
JNIEXPORT jboolean JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_addToCoinTag(JNIEnv *env, jobject thiz,
                                                                  jstring wallet_id, jstring tx_id,
                                                                  jint tag_id, jint vout) {
    try {
        return NunchukProvider::get()->nu->AddToCoinTag(
                StringWrapper(env, wallet_id),
                tag_id,
                StringWrapper(env, tx_id),
                vout);
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
    }
    return false;
}
extern "C"
JNIEXPORT jboolean JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_removeFromCoinTag(JNIEnv *env, jobject thiz,
                                                                       jstring wallet_id,
                                                                       jstring tx_id, jint tag_id,
                                                                       jint vout) {
    try {
        return NunchukProvider::get()->nu->RemoveFromCoinTag(
                StringWrapper(env, wallet_id),
                tag_id,
                StringWrapper(env, tx_id),
                vout);
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
    }
    return false;
}
extern "C"
JNIEXPORT jboolean JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_addToCoinCollection(JNIEnv *env, jobject thiz,
                                                                         jstring wallet_id,
                                                                         jstring tx_id,
                                                                         jint collection_id,
                                                                         jint vout) {
    try {
        return NunchukProvider::get()->nu->AddToCoinCollection(
                StringWrapper(env, wallet_id),
                collection_id,
                StringWrapper(env, tx_id),
                vout);
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
    }
    return false;
}
extern "C"
JNIEXPORT jboolean JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_removeFromCoinCollection(JNIEnv *env,
                                                                              jobject thiz,
                                                                              jstring wallet_id,
                                                                              jstring tx_id,
                                                                              jint collection_id,
                                                                              jint vout) {
    try {
        return NunchukProvider::get()->nu->RemoveFromCoinCollection(
                StringWrapper(env, wallet_id),
                collection_id,
                StringWrapper(env, tx_id),
                vout);
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
    }
    return false;
}
extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_getCoinByTag(JNIEnv *env, jobject thiz,
                                                                  jstring wallet_id,
                                                                  jint tag_id) {
    try {
        auto value = NunchukProvider::get()->nu->GetCoinByTag(StringWrapper(env, wallet_id),
                                                              tag_id);
        return Deserializer::convert2JUnspentOutputs(env, value);
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
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_getCoinInCollection(JNIEnv *env, jobject thiz,
                                                                         jstring wallet_id,
                                                                         jint collection_id) {
    try {
        auto value = NunchukProvider::get()->nu->GetCoinInCollection(StringWrapper(env, wallet_id),
                                                                     collection_id);
        return Deserializer::convert2JUnspentOutputs(env, value);
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
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_getCoinTags(JNIEnv *env, jobject thiz,
                                                                 jstring wallet_id) {
    try {
        auto value = NunchukProvider::get()->nu->GetCoinTags(StringWrapper(env, wallet_id));
        return Deserializer::convert2JCoinTags(env, value);
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
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_createCoinTag(JNIEnv *env, jobject thiz,
                                                                   jstring wallet_id, jstring name,
                                                                   jstring color) {
    try {
        auto value = NunchukProvider::get()->nu->CreateCoinTag(
                StringWrapper(env, wallet_id),
                StringWrapper(env, name),
                StringWrapper(env, color));
        return Deserializer::convert2JCoinTag(env, value);
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return env->ExceptionOccurred();
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return env->ExceptionOccurred();
    }
}
extern "C"
JNIEXPORT jboolean JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_updateCoinTag(JNIEnv *env, jobject thiz,
                                                                   jstring wallet_id,
                                                                   jobject coin_tag) {
    try {
        CoinTag coinTag = Serializer::convert2CCoinTag(env, coin_tag);
        return NunchukProvider::get()->nu->UpdateCoinTag(
                StringWrapper(env, wallet_id),
                coinTag);
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
    }
    return false;
}
extern "C"
JNIEXPORT jboolean JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_deleteCoinTag(JNIEnv *env, jobject thiz,
                                                                   jstring wallet_id, jint tag_id) {
    try {
        return NunchukProvider::get()->nu->DeleteCoinTag(StringWrapper(env, wallet_id), tag_id);
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
    }
    return false;
}
extern "C"
JNIEXPORT jboolean JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_isMyAddress(JNIEnv *env, jobject thiz,
                                                                 jstring wallet_id,
                                                                 jstring address) {
    try {
        return NunchukProvider::get()->nu->IsMyAddress(StringWrapper(env, wallet_id),
                                                       StringWrapper(env, address));
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
    }
    return false;
}