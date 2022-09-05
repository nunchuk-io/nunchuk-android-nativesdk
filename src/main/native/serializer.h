#include <cstring>
#include <jni.h>
#include <syslog.h>
#include <nunchuk.h>
#include <nunchukmatrix.h>
#include "utils/ndef.hpp"

using namespace nunchuk;
using namespace nunchuk::ndef;

/**
 * Used to translate Java properties into C++ equivalents
 */
class Serializer {

public:

    static Device convert2CDevice(JNIEnv *env, jobject device);

    static WalletType convert2CWalletType(jint ordinal);

    static AddressType convert2CAddressType(jint ordinal);

    static SignerType convert2CSignerType(JNIEnv *env, jobject singerType);

    static SatscardSlot::Status convert2CSatsCardSlotStatus(JNIEnv *env, jobject satsCardSlotStatus);

    static Chain convert2CChain(jint ordinal);

    static BackendType convert2CBackendType(jint ordinal);

    static ExportFormat convert2CExportFormat(jint ordinal);

    static SingleSigner convert2CSigner(JNIEnv *env, jobject signer);

    static MasterSigner convert2CMasterSigner(JNIEnv *env, jobject signer);

    static std::vector<SingleSigner> convert2CSigners(JNIEnv *env, jobject signers);

    static std::vector<std::string> convert2CListString(JNIEnv *env, jobject values);

    static Wallet convert2CWallet(JNIEnv *env, jobject wallet);

    static TxInput convert2CTxInput(JNIEnv *env, jobject input);

    static std::vector<TxInput> convert2CTxInputs(JNIEnv *env, jobject inputs);

    static TxOutput convert2CTxOutput(JNIEnv *env, jobject input);

    static std::vector<TxOutput> convert2CTxOutputs(JNIEnv *env, jobject outputs);

    static UnspentOutput convert2CUnspentOutput(JNIEnv *env, jobject unspentOutput);

    static std::vector<UnspentOutput> convert2CUnspentOutputs(JNIEnv *env, jobject unspentOutputs);

    static std::map<std::string, Amount> convert2CAmountsMap(JNIEnv *pEnv, jobject amountsMap);

    static Amount convert2CAmount(JNIEnv *env, jobject amount);

    static NunchukMatrixEvent convert2CMatrixEvent(JNIEnv *env, jobject event);

    static SatscardSlot convert2CSatsCardSlot(JNIEnv *env, jobject slot);

    static std::vector<SatscardSlot> convert2CSatsCardSlots(JNIEnv *env, jobject slots);

    static std::vector<unsigned char> convert2CByteArray(JNIEnv *env, jbyteArray byteArray);

    static std::vector<NDEFRecord> convert2CRecords(JNIEnv *env, jobjectArray records);
};