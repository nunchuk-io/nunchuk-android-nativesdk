#include <cstring>
#include <jni.h>
#include <nunchuk.h>
#include "nunchukprovider.h"
#include "serializer.h"
#include "deserializer.h"
#include "modelprovider.h"
#include "string-wrapper.h"
#include <unordered_map>
#include <memory>
#include <mutex>
#include <vector>

using namespace nunchuk;

// ScriptNode cache for each wallet_id
static std::unordered_map<std::string, std::shared_ptr<nunchuk::ScriptNode>> script_node_cache;
static std::mutex script_node_cache_mutex;

// Helpers to reduce duplication across JNI calls operating on ScriptNode trees
static std::vector<size_t> ToNodePath(JNIEnv *env, jintArray node_id_java) {
    jsize len = env->GetArrayLength(node_id_java);
    std::vector<size_t> path(static_cast<size_t>(len));
    jint *elems = env->GetIntArrayElements(node_id_java, nullptr);
    for (jsize i = 0; i < len; ++i) {
        path[static_cast<size_t>(i)] = static_cast<size_t>(elems[i]);
    }
    env->ReleaseIntArrayElements(node_id_java, elems, JNI_ABORT);
    return path;
}

static std::shared_ptr<nunchuk::ScriptNode> GetOrBuildRootNode(const std::string &wallet_id) {
    std::lock_guard<std::mutex> lock(script_node_cache_mutex);
    auto it = script_node_cache.find(wallet_id);
    if (it != script_node_cache.end()) {
        return it->second;
    }
    auto wallet = NunchukProvider::get()->nu->GetWallet(wallet_id);
    std::string miniscript = wallet.get_miniscript();
    std::vector<std::string> keypath;
    auto root_node = std::make_shared<nunchuk::ScriptNode>(
            nunchuk::Utils::GetScriptNode(miniscript, keypath));
    script_node_cache[wallet_id] = root_node;
    return root_node;
}

static const nunchuk::ScriptNode *GetNodeAtPath(const std::shared_ptr<nunchuk::ScriptNode> &root,
                                                const std::vector<size_t> &path) {
    const nunchuk::ScriptNode *node = root.get();
    for (size_t depth = 1; depth < path.size(); ++depth) {
        size_t idx = path[depth];
        const auto &subs = node->get_subs();
        if (idx == 0 || idx > subs.size()) {
            return nullptr;
        }
        node = &subs[idx - 1];
    }
    return node;
}

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
        jstring replace_tx_id,
        jboolean anti_fee_sniping,
        jboolean use_script_path,
        jobject signing_path
) {
    try {
        auto txInputs = Serializer::convert2CTxInputs(env, inputs);
        auto txInputUnspentOutputs = NunchukProvider::get()->nu->GetUnspentOutputsFromTxInputs(
                StringWrapper(env, wallet_id),
                txInputs);
        SigningPath c_signing_path = Serializer::convert2CSigningPath(env, signing_path);
        auto transaction = NunchukProvider::get()->nu->CreateTransaction(
                env->GetStringUTFChars(wallet_id, JNI_FALSE),
                Serializer::convert2CAmountsMap(env, outputs),
                env->GetStringUTFChars(memo, JNI_FALSE),
                txInputUnspentOutputs,
                Serializer::convert2CAmount(env, fee_rate),
                subtract_fee_from_amount,
                StringWrapper(env, replace_tx_id),
                anti_fee_sniping,
                use_script_path,
                c_signing_path
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
        jstring replace_tx_id,
        jboolean use_script_path,
        jobject signing_path
) {
    try {
        auto c_wallet_id = env->GetStringUTFChars(wallet_id, JNI_FALSE);
        auto txInputs = Serializer::convert2CTxInputs(env, inputs);
        auto txInputUnspentOutputs = NunchukProvider::get()->nu->GetUnspentOutputsFromTxInputs(
                c_wallet_id,
                txInputs);
        SigningPath c_signing_path = Serializer::convert2CSigningPath(env, signing_path);
        auto transaction = NunchukProvider::get()->nu->DraftTransaction(
                c_wallet_id,
                Serializer::convert2CAmountsMap(env, outputs),
                txInputUnspentOutputs,
                Serializer::convert2CAmount(env, fee_rate),
                subtract_fee_from_amount,
                StringWrapper(env, replace_tx_id),
                use_script_path,
                c_signing_path
        );
        Amount packageFeeRate{0};
        jobject jtransaction;
        if (NunchukProvider::get()->nu->IsCPFP(c_wallet_id, transaction,
                                               packageFeeRate)) {
            jtransaction = Deserializer::convert2JTransaction(env, transaction, packageFeeRate);
        } else {
            jtransaction = Deserializer::convert2JTransaction(env, transaction);
        }

        auto wallet = NunchukProvider::get()->nu->GetWallet(c_wallet_id);
        if (wallet.get_address_type() == AddressType::TAPROOT &&
            wallet.get_wallet_type() == WalletType::MULTI_SIG) {
            auto fee = NunchukProvider::get()->nu->GetScriptPathFeeRate(c_wallet_id, transaction);

            jclass txClass = env->GetObjectClass(jtransaction);
            jfieldID feeField = env->GetFieldID(txClass, "scriptPathFee",
                                                "Lcom/nunchuk/android/model/Amount;");
            jobject jfeeAmount = Deserializer::convert2JAmount(env, fee);
            if (feeField != nullptr) {
                env->SetObjectField(jtransaction, feeField, jfeeAmount);
            }
        }

        return jtransaction;
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
        jobject new_fee_rate,
        jboolean anti_fee_sniping,
        jboolean use_script_path,
        jobject signing_path
) {
    try {
        SigningPath c_signing_path = Serializer::convert2CSigningPath(env, signing_path);
        auto transaction = NunchukProvider::get()->nu->ReplaceTransaction(
                env->GetStringUTFChars(wallet_id, JNI_FALSE),
                env->GetStringUTFChars(tx_id, JNI_FALSE),
                Serializer::convert2CAmount(env, new_fee_rate),
                anti_fee_sniping,
                use_script_path,
                c_signing_path
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
                                                                                       jboolean is_draft,
                                                                                       jstring bsms,
                                                                                       jboolean subtract_fee_from_amount) {
    try {
        auto singleSigners = Serializer::convert2CSigners(env, signers);
        Wallet wallet;
        if (bsms != nullptr && env->GetStringLength(bsms) > 0) {
            // Use ParseWalletDescriptor if bsms is provided
            wallet = Utils::ParseWalletDescriptor(
                    StringWrapper(env, bsms));
        } else {
            // Build dummy wallet from signers
            int m = singleSigners.size();
            int n = singleSigners.size();
            wallet = Wallet("", m, n, singleSigners, AddressType::NATIVE_SEGWIT, false, 0, true);
        }
        Transaction tx = Utils::DecodeTx(wallet, StringWrapper(env, psbt),
                                         Utils::AmountFromValue(StringWrapper(env, sub_amount)),
                                         Utils::AmountFromValue(StringWrapper(env, fee)),
                                         Utils::AmountFromValue(StringWrapper(env, fee_rate)),
                                         subtract_fee_from_amount);
        auto draft_tx = Deserializer::convert2JTransaction(env, tx);
        if (is_draft) {
            return draft_tx;
        }
        Transaction signed_tx = tx;
        for (auto &&single_signer: singleSigners) {
            signed_tx = NunchukProvider::get()->nu->SignTransaction(wallet, signed_tx,
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
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_exportBBQRTransaction(JNIEnv *env,
                                                                           jobject thiz,
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
                                                                                jobject fee_rate,
                                                                                jboolean anti_fee_sniping) {
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
                tagIds, collectionIds, Serializer::convert2CAmount(env, fee_rate),
                anti_fee_sniping);
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
JNIEXPORT jint JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_estimateRollOver11TransactionCount(JNIEnv *env,
                                                                                         jobject thiz,
                                                                                         jstring wallet_id) {
    try {
        return NunchukProvider::get()->nu->EstimateRollOver11TransactionCount(
                StringWrapper(env, wallet_id));
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
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_estimateRollOver11Amount(JNIEnv *env,
                                                                               jobject thiz,
                                                                               jstring wallet_id,
                                                                               jstring new_wallet_id,
                                                                               jobject fee_rate,
                                                                               jboolean use_script_path,
                                                                               jobject signing_path) {
    try {
        SigningPath c_signing_path = Serializer::convert2CSigningPath(env, signing_path);
        auto value = NunchukProvider::get()->nu->EstimateRollOver11Amount(
                StringWrapper(env, wallet_id),
                StringWrapper(env, new_wallet_id),
                Serializer::convert2CAmount(env, fee_rate),
                use_script_path,
                c_signing_path);
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
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_draftRollOver11Transactions(JNIEnv *env,
                                                                                  jobject thiz,
                                                                                  jstring wallet_id,
                                                                                  jstring new_wallet_id,
                                                                                  jobject fee_rate,
                                                                                  jboolean use_script_path,
                                                                                  jobject signing_path) {
    try {
        SigningPath c_signing_path = Serializer::convert2CSigningPath(env, signing_path);
        auto txs = NunchukProvider::get()->nu->DraftRollOver11Transactions(
                StringWrapper(env, wallet_id),
                StringWrapper(env, new_wallet_id),
                Serializer::convert2CAmount(env, fee_rate),
                use_script_path,
                c_signing_path);
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
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_createRollOver11Transactions(JNIEnv *env,
                                                                                   jobject thiz,
                                                                                   jstring wallet_id,
                                                                                   jstring new_wallet_id,
                                                                                   jobject fee_rate,
                                                                                   jboolean anti_fee_sniping,
                                                                                   jboolean use_script_path,
                                                                                   jobject signing_path) {
    try {
        SigningPath c_signing_path = Serializer::convert2CSigningPath(env, signing_path);
        auto txs = NunchukProvider::get()->nu->CreateRollOver11Transactions(
                StringWrapper(env, wallet_id),
                StringWrapper(env, new_wallet_id),
                Serializer::convert2CAmount(env, fee_rate),
                anti_fee_sniping,
                use_script_path,
                c_signing_path);
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
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_draftRbfTransaction(JNIEnv *env, jobject thiz,
                                                                         jstring wallet_id,
                                                                         jobject fee_rate,
                                                                         jstring replace_tx_id,
                                                                         jboolean use_script_path,
                                                                         jobject signing_path) {
    try {
        auto c_wallet_id = StringWrapper(env, wallet_id);
        auto original_tx = NunchukProvider::get()->nu->GetTransaction(
                StringWrapper(env, wallet_id),
                StringWrapper(env, replace_tx_id));
        auto txInputUnspentOutputs = NunchukProvider::get()->nu->GetUnspentOutputsFromTxInputs(
                c_wallet_id,
                original_tx.get_inputs());
        auto mapOut = std::map<std::string, Amount>();
        for (const auto &output: original_tx.get_user_outputs()) {
            mapOut[output.first] = output.second;
        }
        SigningPath c_signing_path = Serializer::convert2CSigningPath(env, signing_path);
        auto transaction = NunchukProvider::get()->nu->DraftTransaction(
                c_wallet_id,
                mapOut,
                txInputUnspentOutputs,
                Serializer::convert2CAmount(env, fee_rate),
                original_tx.subtract_fee_from_amount(),
                StringWrapper(env, replace_tx_id),
                use_script_path,
                c_signing_path
        );
        Amount packageFeeRate{0};
        jobject jtransaction;
        if (NunchukProvider::get()->nu->IsCPFP(c_wallet_id, transaction,
                                               packageFeeRate)) {
            jtransaction = Deserializer::convert2JTransaction(env, transaction, packageFeeRate);
        } else {
            jtransaction = Deserializer::convert2JTransaction(env, transaction);
        }

        auto wallet = NunchukProvider::get()->nu->GetWallet(c_wallet_id);
        if (wallet.get_address_type() == AddressType::TAPROOT && wallet.get_wallet_type() == WalletType::MULTI_SIG) {
            auto fee = NunchukProvider::get()->nu->GetScriptPathFeeRate(c_wallet_id, transaction);

            jclass txClass = env->GetObjectClass(jtransaction);
            jfieldID feeField = env->GetFieldID(txClass, "scriptPathFee",
                                                "Lcom/nunchuk/android/model/Amount;");
            jobject jfeeAmount = Deserializer::convert2JAmount(env, fee);
            if (feeField != nullptr) {
                env->SetObjectField(jtransaction, feeField, jfeeAmount);
            }
        }

        return jtransaction;
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
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_getTimelockedCoins(
        JNIEnv *env,
        jobject thiz,
        jstring wallet_id,
        jobject inputs
) {
    try {
        // Convert parameters
        auto c_wallet_id = StringWrapper(env, wallet_id);
        auto tx_inputs = Serializer::convert2CTxInputs(env, inputs);
        auto wallet = NunchukProvider::get()->nu->GetWallet(c_wallet_id);
        std::vector<nunchuk::UnspentOutput> coins = NunchukProvider::get()->nu->GetCoinsFromTxInputs(
                wallet.get_id(), tx_inputs);
        int64_t max_lock_value = 0;
        std::vector<nunchuk::UnspentOutput> locked_coins = nunchuk::Utils::GetTimelockedCoins(
                wallet.get_miniscript(), coins, max_lock_value,
                NunchukProvider::get()->nu->GetChainTip());
        jobject lockedCoinsList = Deserializer::convert2JUnspentOutputs(env, locked_coins);
        jclass pairClass = env->FindClass("kotlin/Pair");
        jmethodID pairConstructor = env->GetMethodID(pairClass, "<init>",
                                                     "(Ljava/lang/Object;Ljava/lang/Object;)V");
        jobject maxLockValueObj = env->NewObject(env->FindClass("java/lang/Long"),
                                                 env->GetMethodID(env->FindClass("java/lang/Long"),
                                                                  "<init>", "(J)V"),
                                                 static_cast<jlong>(max_lock_value));
        jobject pairObj = env->NewObject(pairClass, pairConstructor, maxLockValueObj,
                                         lockedCoinsList);
        env->DeleteLocalRef(maxLockValueObj);
        env->DeleteLocalRef(lockedCoinsList);
        return pairObj;
    } catch (const std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return nullptr;
    }
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_estimateFeeForSigningPaths(
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
        auto c_wallet_id = StringWrapper(env, wallet_id);
        auto c_outputs = Serializer::convert2CAmountsMap(env, outputs);
        auto txInputs = Serializer::convert2CTxInputs(env, inputs);
        auto txInputUnspentOutputs = NunchukProvider::get()->nu->GetUnspentOutputsFromTxInputs(
                c_wallet_id,
                txInputs);
        Amount c_fee_rate = Serializer::convert2CAmount(env, fee_rate);
        auto c_replace_tx_id = StringWrapper(env, replace_tx_id);
        auto result = NunchukProvider::get()->nu->EstimateFeeForSigningPaths(
                c_wallet_id, c_outputs, txInputUnspentOutputs, c_fee_rate, subtract_fee_from_amount,
                c_replace_tx_id);
        return Deserializer::convert2JSigningPathAmountPairs(env, result);
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
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_isPreimageRevealed(
        JNIEnv *env,
        jobject thiz,
        jstring wallet_id,
        jstring tx_id,
        jbyteArray hash
) {
    try {
        auto c_wallet_id = StringWrapper(env, wallet_id);
        auto c_tx_id = StringWrapper(env, tx_id);

        // Get transaction to access PSBT or raw transaction
        auto tx = NunchukProvider::get()->nu->GetTransaction(c_wallet_id, c_tx_id);
        
        // Convert hash byte array
        jsize hash_len = env->GetArrayLength(hash);
        std::vector<uint8_t> hash_vec(hash_len);
        env->GetByteArrayRegion(hash, 0, hash_len, reinterpret_cast<jbyte *>(hash_vec.data()));

        bool result;
        
        // For finalized transaction (!tx.get_raw().empty()), check hash from raw transaction
        std::string rawtx = tx.get_raw();
        if (!rawtx.empty()) {
            result = nunchuk::Utils::IsPreimageRevealed(rawtx, hash_vec);
        } else {
            // For non-finalized transaction, check hash from PSBT
            std::string psbt = tx.get_psbt();
            result = nunchuk::Utils::IsPreimageRevealed(psbt, hash_vec);
        }
        
        return result ? JNI_TRUE : JNI_FALSE;
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return JNI_FALSE;
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return JNI_FALSE;
    }
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_revealPreimage(
        JNIEnv *env,
        jobject thiz,
        jstring wallet_id,
        jstring tx_id,
        jbyteArray hash,
        jbyteArray preimage
) {
    try {
        auto c_wallet_id = StringWrapper(env, wallet_id);
        auto c_tx_id = StringWrapper(env, tx_id);
        std::vector<uint8_t> hash_vec = Serializer::convert2CByteArray(env, hash);
        std::vector<uint8_t> preimage_vec = Serializer::convert2CByteArray(env, preimage);

        bool result = NunchukProvider::get()->nu->RevealPreimage(c_wallet_id, c_tx_id, hash_vec,
                                                               preimage_vec);
        return result ? JNI_TRUE : JNI_FALSE;
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return JNI_FALSE;
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return JNI_FALSE;
    }
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_isSatisfiable(
        JNIEnv *env,
        jobject thiz,
        jstring wallet_id,
        jintArray node_id,
        jstring psbt
) {
    try {
        auto c_wallet_id = StringWrapper(env, wallet_id);
        auto c_psbt = StringWrapper(env, psbt);
        auto node_path = ToNodePath(env, node_id);
        auto root_node_ptr = GetOrBuildRootNode(c_wallet_id);
        const nunchuk::ScriptNode *node = GetNodeAtPath(root_node_ptr, node_path);
        if (node == nullptr) return JNI_FALSE;

        // Call is_satisfiable
        bool result = node->is_satisfiable(c_psbt);
        return result ? JNI_TRUE : JNI_FALSE;
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
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_getKeySetStatus(
        JNIEnv *env,
        jobject thiz,
        jstring wallet_id,
        jintArray node_id,
        jstring tx_id
) {
    try {
        std::string c_wallet_id = env->GetStringUTFChars(wallet_id, JNI_FALSE);
        std::string c_tx_id = env->GetStringUTFChars(tx_id, JNI_FALSE);
        auto node_path = ToNodePath(env, node_id);
        auto root_node_ptr = GetOrBuildRootNode(c_wallet_id);
        const nunchuk::ScriptNode *node = GetNodeAtPath(root_node_ptr, node_path);
        if (node == nullptr) return nullptr;
        auto tx = NunchukProvider::get()->nu->GetTransaction(c_wallet_id, c_tx_id);

        // Get keyset status from node
        nunchuk::KeysetStatus ks = node->get_keyset_status(tx);

        // Build KeySetStatus Kotlin object via converter
        return Deserializer::convert2JKeySetStatusSingle(env, ks);
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
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_getCoinsGroupedBySubPolicies(
        JNIEnv *env,
        jobject thiz,
        jstring wallet_id,
        jintArray node_id,
        jstring tx_id
) {
    try {
        std::string c_wallet_id = env->GetStringUTFChars(wallet_id, JNI_FALSE);
        std::string c_tx_id = env->GetStringUTFChars(tx_id, JNI_FALSE);
        auto node_path = ToNodePath(env, node_id);
        auto root_node_ptr = GetOrBuildRootNode(c_wallet_id);
        const nunchuk::ScriptNode *node = GetNodeAtPath(root_node_ptr, node_path);
        if (node == nullptr) return nullptr;
        auto tx = NunchukProvider::get()->nu->GetTransaction(c_wallet_id, c_tx_id);
        auto coins = NunchukProvider::get()->nu->GetCoinsFromTxInputs(c_wallet_id, tx.get_inputs());

        // Group coins by sub-policies
        int chain_tip = NunchukProvider::get()->nu->GetChainTip();
        auto groups = nunchuk::Utils::GetCoinsGroupedBySubPolicies(*node, coins, chain_tip);

        // Convert to Kotlin List<CoinsGroup>
        return Deserializer::convert2JCoinsGroups(env, groups);
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
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_getTimelockedUntil(
        JNIEnv *env,
        jobject thiz,
        jstring wallet_id,
        jstring tx_id
) {
    try {
        auto c_wallet_id = StringWrapper(env, wallet_id);
        auto c_tx_id = StringWrapper(env, tx_id);
        auto result = NunchukProvider::get()->nu->GetTimelockedUntil(c_wallet_id, c_tx_id);
        return Deserializer::convert2JPairLongMiniscriptTimelockBased(env, result);
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
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_clearScriptNodeCache(
        JNIEnv *env,
        jobject thiz,
        jstring wallet_id
) {
    try {
        std::string c_wallet_id = env->GetStringUTFChars(wallet_id, JNI_FALSE);
        std::lock_guard<std::mutex> lock(script_node_cache_mutex);
        script_node_cache.erase(c_wallet_id);
        env->ReleaseStringUTFChars(wallet_id, c_wallet_id.c_str());
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
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_getTransactionSigners(
        JNIEnv *env,
        jobject thiz,
        jstring wallet_id,
        jstring tx_id
) {
    try {
        auto c_wallet_id = StringWrapper(env, wallet_id);
        auto c_tx_id = StringWrapper(env, tx_id);
        
        auto signers = NunchukProvider::get()->nu->GetTransactionSigners(c_wallet_id, c_tx_id);
        
        return Deserializer::convert2JSigners(env, signers);
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return nullptr;
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return nullptr;
    }
}