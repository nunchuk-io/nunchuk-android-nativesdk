#include <jni.h>
#include "deserializer.h"
#include "serializer.h"
#include "nunchukprovider.h"
#include "utils/ndef.hpp"
#include "string-wrapper.h"
#include "utils/coldcard.hpp"

using namespace nunchuk::ndef;
using namespace nunchuk;

extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_getMk4Signers(JNIEnv *env, jobject thiz,
                                                                   jobjectArray records) {
    try {
        auto cRecords = Serializer::convert2CRecords(env, records);
        NDEFMessageType type = DetectNDEFMessageType(cRecords);
        auto signers = std::vector<SingleSigner>(0);
        if (type == NDEFMessageType::JSON) {
            std::string json_data = NDEFRecordToJSON(cRecords[0]);
            signers = NunchukProvider::get()->nu->ParseJSONSigners(json_data);
        }
        return Deserializer::convert2JSigners(env, signers);
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
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_createMk4Signer(JNIEnv *env, jobject thiz,
                                                                     jobject signer) {
    try {
        auto cSigner = Serializer::convert2CSigner(env, signer);
        auto newSigner = NunchukProvider::get()->nu->CreateSigner(
                cSigner.get_name(),
                cSigner.get_xpub(),
                cSigner.get_public_key(),
                cSigner.get_derivation_path(),
                cSigner.get_master_fingerprint(),
                cSigner.get_type()
        );
        return Deserializer::convert2JSigner(env, newSigner);
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return JNI_FALSE;
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return JNI_FALSE;
    }
}
extern "C"
JNIEXPORT jobjectArray JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_exportWalletToMk4(JNIEnv *env, jobject thiz,
                                                                       jstring wallet_id) {
    try {
        auto data = NunchukProvider::get()->nu->GetWalletExportData(StringWrapper(env, wallet_id),
                                                                    ExportFormat::COLDCARD);
        auto cRecords = NDEFRecordsFromStr(data);
        return Deserializer::convert2JRecords(env, cRecords);
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return JNI_FALSE;
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return JNI_FALSE;
    }
}
extern "C"
JNIEXPORT jobjectArray JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_exportPsbtToMk4(JNIEnv *env, jobject thiz,
                                                                     jstring wallet_id,
                                                                     jstring tx_id) {
    try {
        auto tx = NunchukProvider::get()->nu->GetTransaction(StringWrapper(env, wallet_id),
                                                             StringWrapper(env, tx_id));
        auto cRecords = NDEFRecordsFromPSBT(tx.get_psbt());
        return Deserializer::convert2JRecords(env, cRecords);
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return JNI_FALSE;
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return JNI_FALSE;
    }
}
extern "C"
JNIEXPORT jobjectArray JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_exportRawPsbtToMk4(JNIEnv *env, jobject thiz,
                                                                     jstring psbt) {
    try {
        auto cRecords = NDEFRecordsFromPSBT(StringWrapper(env, psbt));
        return Deserializer::convert2JRecords(env, cRecords);
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
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_importTransactionFromMk4(JNIEnv *env,
                                                                              jobject thiz,
                                                                              jstring wallet_id,
                                                                              jobjectArray records) {
    try {
        auto cRecords = Serializer::convert2CRecords(env, records);
        NDEFMessageType type = DetectNDEFMessageType(cRecords);
        if (type == NDEFMessageType::PSBT) {
            auto psbt = NDEFRecordsToPSBT(cRecords);
            auto tx = NunchukProvider::get()->nu->ImportPsbt(StringWrapper(env, wallet_id), psbt);
            return Deserializer::convert2JTransaction(env, tx);
        } else if (type == NDEFMessageType::TRANSACTION) {
            auto raw_tx = NDEFRecordsToRawTransaction(cRecords);
            auto tx = NunchukProvider::get()->nu->ImportRawTransaction(
                    StringWrapper(env, wallet_id), raw_tx);
            return Deserializer::convert2JTransaction(env, tx);
        }
        return nullptr;
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
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_importWalletFromMk4(JNIEnv *env, jobject thiz,
                                                                         jint chain,
                                                                         jobjectArray records) {
    try {
        auto cRecords = Serializer::convert2CRecords(env, records);
        NDEFMessageType type = DetectNDEFMessageType(cRecords);
        if (type == NDEFMessageType::WALLET) {
            auto config = NDEFRecordToStr(cRecords[0]);
            auto cChain = Serializer::convert2CChain(chain);
            auto wallet = Utils::ParseWalletConfig(cChain, config);
            auto result = NunchukProvider::get()->nu->CreateWallet(wallet, true);
            return Deserializer::convert2JWallet(env, result);
        }
        return nullptr;
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
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_getColdCardWallets(JNIEnv *env, jobject thiz,
                                                                        jobjectArray records) {
    try {
        auto cRecords = Serializer::convert2CRecords(env, records);
        NDEFMessageType type = DetectNDEFMessageType(cRecords);
        if (type == NDEFMessageType::JSON) {
            std::string json_data = NDEFRecordToJSON(cRecords[0]);
            auto wallets = NunchukProvider::get()->nu->ParseJSONWallets(json_data);
            return Deserializer::convert2JWallets(env, wallets);
        }
        return nullptr;
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
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_createWallet2(JNIEnv *env, jobject thiz,
                                                                   jobject wallet) {
    try {
        auto cWallet = Serializer::convert2CWallet(env, wallet);
        auto result = NunchukProvider::get()->nu->CreateWallet(cWallet, true);
        return Deserializer::convert2JWallet(env, result);
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
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_generateColdCardHealthCheckMessage(JNIEnv *env,
                                                                                        jobject thiz,
                                                                                        jstring derivation_path,
                                                                                        jstring message) {
    try {
        auto cMessage = message != nullptr ? StringWrapper(env, derivation_path)
                                           : Utils::GenerateHealthCheckMessage();
        auto data = GenerateColdCardHealthCheckMessage(
                StringWrapper(env, derivation_path),
                cMessage
        );
        auto cRecords = NDEFRecordsFromStr(data);
        return Deserializer::convert2JRecords(env, cRecords);
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
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_healthCheckColdCard(JNIEnv *env, jobject thiz,
                                                                         jobject signer,
                                                                         jobjectArray records) {
    try {
        auto cRecords = Serializer::convert2CRecords(env, records);
        NDEFMessageType type = DetectNDEFMessageType(cRecords);
        if (type == NDEFMessageType::TEXT) {
            std::string text = NDEFRecordToStr(cRecords[0]);
            BitcoinSignedMessage signed_message = ParseBitcoinSignedMessage(text);
            SingleSigner cSinger = Serializer::convert2CSigner(env, signer);
            HealthStatus status = NunchukProvider::get()->nu->HealthCheckSingleSigner(cSinger,
                                                                                      signed_message.message,
                                                                                      signed_message.signature);
            return Deserializer::convert2JColdCardHealth(env, status, signed_message.signature);
        }
        return nullptr;
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return nullptr;
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return nullptr;
    }
}
extern "C"
JNIEXPORT jstring JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_signMessageColdCard(JNIEnv *env, jobject thiz,
                                                                         jstring derivation_path,
                                                                         jstring messages_to_sign) {
    try {
        std::string generate_msg = GenerateColdCardHealthCheckMessage(
                StringWrapper(env, derivation_path), StringWrapper(env, messages_to_sign));
        auto records = NDEFRecordsFromStr(generate_msg);
        std::string str = NDEFRecordToStr(records[0]);
        BitcoinSignedMessage signed_message = ParseBitcoinSignedMessage(str);
        return env->NewStringUTF(signed_message.signature.c_str());
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return nullptr;
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return nullptr;
    }
}
extern "C"
JNIEXPORT jstring JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_getColdcardSignatureFromPsbt(JNIEnv *env,
                                                                                  jobject thiz,
                                                                                  jobject signer,
                                                                                  jobjectArray records) {
    try {
        auto cRecords = Serializer::convert2CRecords(env, records);
        NDEFMessageType type = DetectNDEFMessageType(cRecords);
        if (type == NDEFMessageType::PSBT) {
            auto psbt = NDEFRecordsToPSBT(cRecords);
            auto singleSigner = Serializer::convert2CSigner(env, signer);
            auto signature = Utils::GetPartialSignature(singleSigner, psbt);
            return env->NewStringUTF(signature.c_str());
        }
        return nullptr;
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
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_parseWalletFromMk4(JNIEnv *env, jobject thiz,
                                                                        jint chain,
                                                                        jobjectArray records) {
    try {
        auto cRecords = Serializer::convert2CRecords(env, records);
        NDEFMessageType type = DetectNDEFMessageType(cRecords);
        if (type == NDEFMessageType::WALLET) {
            auto config = NDEFRecordToStr(cRecords[0]);
            auto cChain = Serializer::convert2CChain(chain);
            auto wallet = Utils::ParseWalletConfig(cChain, config);
            return Deserializer::convert2JWallet(env, wallet);
        }
        return nullptr;
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
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_verifyColdCardBackup(JNIEnv *env, jobject thiz,
                                                                          jbyteArray data,
                                                                          jstring back_up_key,
                                                                          jstring master_signer_id) {
    try {
        auto cData = Serializer::convert2CByteArray(env, data);
        auto back_up_key_str = StringWrapper(env, back_up_key);
        auto master_signer_id_str = StringWrapper(env, master_signer_id);
        NunchukProvider::get()->nu->VerifyColdcardBackup(cData, back_up_key_str, master_signer_id_str);
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
    }
}