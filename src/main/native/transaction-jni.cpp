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
        jboolean subtract_fee_from_amount,
        jstring replace_tx_id
) {
    try {
        auto txInputs = Serializer::convert2CTxInputs(env, inputs);
        auto txInputUnspentOutputs = NunchukProvider::get()->nu->GetUnspentOutputsFromTxInputs(
                StringWrapper(env, wallet_id),
                txInputs);
        auto transaction = NunchukProvider::get()->nu->CreateTransaction(
                env->GetStringUTFChars(wallet_id, JNI_FALSE),
                Serializer::convert2CAmountsMap(env, outputs),
                env->GetStringUTFChars(memo, JNI_FALSE),
                txInputUnspentOutputs,
                Serializer::convert2CAmount(env, fee_rate),
                subtract_fee_from_amount,
                StringWrapper(env, replace_tx_id)
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
        jboolean subtract_fee_from_amount,
        jstring replace_tx_id
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
                subtract_fee_from_amount,
                StringWrapper(env, replace_tx_id)
        );
        Amount packageFeeRate{0};
        if (NunchukProvider::get()->nu->IsCPFP(StringWrapper(env, wallet_id), transaction,
                                               packageFeeRate)) {
            return Deserializer::convert2JTransaction(env, transaction, packageFeeRate);
        } else {
            return Deserializer::convert2JTransaction(env, transaction);
        }
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
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_createInheritanceClaimTransaction(JNIEnv *env,
                                                                                       jobject thiz,
                                                                                       jobject signers,
                                                                                       jstring psbt,
                                                                                       jstring sub_amount,
                                                                                       jstring fee_rate,
                                                                                       jstring fee,
                                                                                       jboolean is_draft) {
    try {
        Wallet dummy_wallet = Wallet(false);
        auto singleSigners = Serializer::convert2CSigners(env, signers);
        dummy_wallet.set_signers(singleSigners);
        Transaction tx = Utils::DecodeTx(dummy_wallet, StringWrapper(env, psbt),
                                         Utils::AmountFromValue(StringWrapper(env, sub_amount)),
                                         Utils::AmountFromValue(StringWrapper(env, fee)),
                                         Utils::AmountFromValue(StringWrapper(env, fee_rate)));
        auto draft_tx = Deserializer::convert2JTransaction(env, tx);
        if (is_draft) {
            return draft_tx;
        }
        Transaction signed_tx = tx;
        for (auto &&single_signer: singleSigners) {
            signed_tx = NunchukProvider::get()->nu->SignTransaction(dummy_wallet, signed_tx,
                                                                    Device(single_signer.get_master_fingerprint()));
        }
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
extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_createCoinCollection(JNIEnv *env, jobject thiz,
                                                                          jstring wallet_id,
                                                                          jstring name) {
    try {
        auto value = NunchukProvider::get()->nu->CreateCoinCollection(
                StringWrapper(env, wallet_id),
                StringWrapper(env, name));
        return Deserializer::convert2JCoinCollection(env, value);
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
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_getCoinCollections(JNIEnv *env, jobject thiz,
                                                                        jstring wallet_id) {
    try {
        auto value = NunchukProvider::get()->nu->GetCoinCollections(StringWrapper(env, wallet_id));
        return Deserializer::convert2JCoinCollections(env, value);
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
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_updateCoinCollection(JNIEnv *env, jobject thiz,
                                                                          jstring wallet_id,
                                                                          jobject coin_collection,
                                                                          jboolean apply_to_existing_coins
                                                                          ) {
    try {
        CoinCollection coinCollection = Serializer::convert2CCoinCollection(env, coin_collection);
        return NunchukProvider::get()->nu->UpdateCoinCollection(
                StringWrapper(env, wallet_id),
                coinCollection,
                apply_to_existing_coins);
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
    }
    return false;
}
extern "C"
JNIEXPORT jboolean JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_deleteCoinCollection(JNIEnv *env, jobject thiz,
                                                                          jstring wallet_id,
                                                                          jint collection_id) {
    try {
        return NunchukProvider::get()->nu->DeleteCoinCollection(StringWrapper(env, wallet_id),
                                                                collection_id);
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
    }
    return false;
}
extern "C"
JNIEXPORT jstring JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_getRawTransaction(JNIEnv *env, jobject thiz,
                                                                       jstring wallet_id,
                                                                       jstring tx_id) {
    try {
        auto value = NunchukProvider::get()->nu->GetRawTransaction(
                StringWrapper(env, wallet_id),
                StringWrapper(env, tx_id));
        return env->NewStringUTF(value.c_str());
    } catch (BaseException &e) {
        return env->NewStringUTF("");
    } catch (std::exception &e) {
        return env->NewStringUTF("");
    }
}
extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_importDummyTx(JNIEnv *env, jobject thiz,
                                                                   jstring dummy_transaction_json) {
    try {
        auto pair = NunchukProvider::get()->nu->ImportDummyTx(
                StringWrapper(env, dummy_transaction_json));
        pair.second.set_txid(pair.first);
        return Deserializer::convert2JTransaction(env, pair.second);
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
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_deleteDummyTx(JNIEnv *env, jobject thiz,
                                                                   jstring wallet_id,
                                                                   jstring tx_id) {
    try {
        return NunchukProvider::get()->nu->DeleteDummyTx(
                StringWrapper(env, wallet_id),
                StringWrapper(env, tx_id));
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
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_saveDummyTxRequestToken(JNIEnv *env,
                                                                             jobject thiz,
                                                                             jstring wallet_id,
                                                                             jstring tx_id,
                                                                             jstring request_token) {
    try {
        auto value = NunchukProvider::get()->nu->SaveDummyTxRequestToken(
                StringWrapper(env, wallet_id),
                StringWrapper(env, tx_id),
                StringWrapper(env, request_token));
        return Deserializer::convert2JStringBooleanMap(env, value);
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
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_getDummyTxRequestToken(JNIEnv *env,
                                                                            jobject thiz,
                                                                            jstring wallet_id,
                                                                            jstring tx_id) {
    try {
        auto value = NunchukProvider::get()->nu->GetDummyTxRequestToken(
                StringWrapper(env, wallet_id),
                StringWrapper(env, tx_id));
        return Deserializer::convert2JStringBooleanMap(env, value);
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
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_getDummyTransactions(JNIEnv *env, jobject thiz,
                                                                          jstring wallet_id) {
    try {
        auto map = NunchukProvider::get()->nu->GetDummyTxs(StringWrapper(env, wallet_id));
        std::vector<Transaction> transactions;
        for (auto &pair: map) {
            pair.second.set_txid(pair.first);
            transactions.push_back(pair.second);
        }
        return Deserializer::convert2JTransactions(env, transactions);
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
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_getDummyTransaction(JNIEnv *env, jobject thiz,
                                                                         jstring wallet_id,
                                                                         jstring tx_id) {
    try {
        auto tx = NunchukProvider::get()->nu->GetDummyTx(
                StringWrapper(env, wallet_id),
                StringWrapper(env, tx_id));
        tx.set_txid(StringWrapper(env, tx_id));
        return Deserializer::convert2JTransaction(env, tx);
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
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_getCoinsFromTxInputs(JNIEnv *env, jobject thiz,
                                                                          jstring wallet_id,
                                                                          jobject inputs) {
    try {
        auto value = NunchukProvider::get()->nu->GetCoinsFromTxInputs(
                StringWrapper(env, wallet_id),
                Serializer::convert2CTxInputs(env, inputs));
        return Deserializer::convert2JUnspentOutputs(env, value);
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
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_replaceTransactionId(JNIEnv *env, jobject thiz,
                                                                          jstring wallet_id,
                                                                          jstring tx_id,
                                                                          jstring replace_tx_id) {
    try {
        NunchukProvider::get()->nu->ReplaceTransactionId(
                StringWrapper(env, wallet_id),
                StringWrapper(env, tx_id),
                StringWrapper(env, replace_tx_id));
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
    }
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_exportBBQRTransaction(JNIEnv *env, jobject thiz,
                                                                           jstring psbt,
                                                                           jint density) {
    try {
        auto value = Utils::ExportBBQRTransaction(StringWrapper(env, psbt), 1, density);
        return Deserializer::convert2JListString(env, value);
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return nullptr;
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return nullptr;
    }
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_estimateRollOverTransactionCount(JNIEnv *env,
                                                                                      jobject thiz,
                                                                                      jstring wallet_id,
                                                                                      jobject tags,
                                                                                      jobject collections) {
    try {
        auto coinTags = Serializer::convert2CCoinTags(env, tags);
        auto tagIds = std::set<int>();
        for (auto &tag: coinTags) {
            tagIds.insert(tag.get_id());
        }
        auto coinCollections = Serializer::convert2CCoinCollections(env, collections);
        auto collectionIds = std::set<int>();
        for (auto &collection: coinCollections) {
            collectionIds.insert(collection.get_id());
        }
        return NunchukProvider::get()->nu->EstimateRollOverTransactionCount(
                StringWrapper(env, wallet_id),
                tagIds, collectionIds);
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return -1;
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return -1;
    }
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_estimateRollOverAmount(JNIEnv *env,
                                                                            jobject thiz,
                                                                            jstring wallet_id,
                                                                            jstring new_wallet_id,
                                                                            jobject tags,
                                                                            jobject collections,
                                                                            jobject fee_rate) {
    try {
        auto coinTags = Serializer::convert2CCoinTags(env, tags);
        auto tagIds = std::set<int>();
        for (auto &tag: coinTags) {
            tagIds.insert(tag.get_id());
        }
        auto coinCollections = Serializer::convert2CCoinCollections(env, collections);
        auto collectionIds = std::set<int>();
        for (auto &collection: coinCollections) {
            collectionIds.insert(collection.get_id());
        }
        auto value = NunchukProvider::get()->nu->EstimateRollOverAmount(
                StringWrapper(env, wallet_id),
                StringWrapper(env, new_wallet_id),
                tagIds, collectionIds,
                Serializer::convert2CAmount(env, fee_rate));
        return Deserializer::convert2JPairAmount(env, value);
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return nullptr;
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return nullptr;
    }
}
extern "C"
JNIEXPORT jobjectArray JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_draftRollOverTransactions(JNIEnv *env,
                                                                               jobject thiz,
                                                                               jstring wallet_id,
                                                                               jstring new_wallet_id,
                                                                               jobject tags,
                                                                               jobject collections,
                                                                               jobject fee_rate) {
    try {
        auto coinTags = Serializer::convert2CCoinTags(env, tags);
        auto tagIds = std::set<int>();
        for (auto &tag: coinTags) {
            tagIds.insert(tag.get_id());
        }
        auto coinCollections = Serializer::convert2CCoinCollections(env, collections);
        auto collectionIds = std::set<int>();
        for (auto &collection: coinCollections) {
            collectionIds.insert(collection.get_id());
        }
        auto txs = NunchukProvider::get()->nu->DraftRollOverTransactions(
                StringWrapper(env, wallet_id),
                StringWrapper(env, new_wallet_id),
                tagIds, collectionIds, Serializer::convert2CAmount(env, fee_rate));
        return Deserializer::convert2JDraftRollOverTransactions(env, txs);
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
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_createRollOverTransactions(JNIEnv *env,
                                                                                jobject thiz,
                                                                                jstring wallet_id,
                                                                                jstring new_wallet_id,
                                                                                jobject tags,
                                                                                jobject collections,
                                                                                jobject fee_rate) {
    try {
        auto coinTags = Serializer::convert2CCoinTags(env, tags);
        auto tagIds = std::set<int>();
        for (auto &tag: coinTags) {
            tagIds.insert(tag.get_id());
        }
        auto coinCollections = Serializer::convert2CCoinCollections(env, collections);
        auto collectionIds = std::set<int>();
        for (auto &collection: coinCollections) {
            collectionIds.insert(collection.get_id());
        }
        auto txs = NunchukProvider::get()->nu->CreateRollOverTransactions(
                StringWrapper(env, wallet_id),
                StringWrapper(env, new_wallet_id),
                tagIds, collectionIds, Serializer::convert2CAmount(env, fee_rate));
        return Deserializer::convert2JTransactions(env, txs);
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
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_getScriptPathFeeRate(JNIEnv *env, jobject thiz,
                                                                          jstring wallet_id,
                                                                          jstring transaction_id) {
    try {
        std::string c_wallet_id = StringWrapper(env, wallet_id);
        std::string c_tx_id = StringWrapper(env, transaction_id);

        auto tx = NunchukProvider::get()->nu->GetTransaction(c_wallet_id, c_tx_id);
        nunchuk::Amount fee_rate = NunchukProvider::get()->nu->GetScriptPathFeeRate(c_wallet_id, tx);

        return Deserializer::convert2JAmount(env, fee_rate);
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return nullptr;
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return nullptr;
    }
}