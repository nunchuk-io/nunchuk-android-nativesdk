#include <cstring>
#include <jni.h>
#include <syslog.h>
#include <nunchuk.h>
#include <nunchukmatrix.h>
#include "utils/ndef.hpp"

using namespace nunchuk;
using namespace nunchuk::ndef;

/**
 * Used to convert C++ Classes into Java models
 */
class Deserializer {

public:

    static jobject convert2JBoolean(JNIEnv *env, const bool value);

    static jobject convert2JListString(JNIEnv *env, const std::vector<std::string> &values);

    static jobject convert2JStringBooleanMap(JNIEnv *env, const std::map<std::string, bool> &signersMap);

    static void convert2JException(JNIEnv *env, const BaseException &e);

    static void convertStdException2JException(JNIEnv *env, const std::exception &e);

    static jobject convert2JDevice(JNIEnv *env, const Device &device);

    static jobject convert2JDevices(JNIEnv *env, const std::vector<Device> &devices);

    static jobject convert2JAmount(JNIEnv *env, const Amount amount);

    static jobject convert2JTxInput(JNIEnv *env, const TxInput &input);

    static jobject convert2JTxInputs(JNIEnv *env, const std::vector<TxInput> &inputs);

    static jobject convert2JTxOutput(JNIEnv *env, const TxOutput &output);

    static jobject convert2JTxOutputs(JNIEnv *env, const std::vector<TxOutput> &outputs);

    static jobject convert2JAddressType(JNIEnv *env, const AddressType &type);

    static jobject convert2JSignerType(JNIEnv *env, const SignerType &type);

    static jobject convert2JTransactionStatus(JNIEnv *env, const TransactionStatus &status);

    static jobject convert2JMasterSigner(JNIEnv *env, const MasterSigner &signer);

    static jobject convert2JMasterSigners(JNIEnv *env, const std::vector<MasterSigner> &signers);

    static jobject convert2JSigner(JNIEnv *env, const SingleSigner &signer);

    static jobject convert2JSigners(JNIEnv *env, const std::vector<SingleSigner> &signers);

    static jobject convert2JWallet(JNIEnv *env, const Wallet &wallet);

    static jobject convert2JWallets(JNIEnv *env, const std::vector<Wallet> &wallets);

    static jobject convert2JTransaction(JNIEnv *env, const Transaction &transaction);

    static jobject convert2JTransaction(JNIEnv *env, const Transaction &transaction, const Amount amount);

    static jobject convert2JTransactions(JNIEnv *env, const std::vector<Transaction> &transactions);

    static jobject convert2JMatrixEvent(JNIEnv *env, const NunchukMatrixEvent &event);

    static jobject convert2JRoomWallet(JNIEnv *env, const RoomWallet &wallet);

    static jobject convert2JRoomWallets(JNIEnv *env, const std::vector<RoomWallet> &wallets);

    static jobject convert2JRoomTransaction(JNIEnv *env, const RoomTransaction &transaction);

    static jobject convert2JRoomTransactions(JNIEnv *env, const std::vector<RoomTransaction> &transactions);

    static jobject convert2JConnectionStatus(JNIEnv *env, const ConnectionStatus &status);

    static jobject convert2JHealthStatus(JNIEnv *env, const HealthStatus &status);

    static jobject convert2JColdCardHealth(JNIEnv *env, const HealthStatus &status, const std::string signature);

    static jobject convert2JTapSignerStatus(JNIEnv *env, const TapsignerStatus &status);

    static jobject convert2JSatsCardSlotStatus(JNIEnv *env, const SatscardSlot::Status &status);

    static jobject convert2JSatsCardSlot(JNIEnv *env, const SatscardSlot &slot);

    static jobject convert2JSatsCardSlots(JNIEnv *env, const std::vector<SatscardSlot> &slots);

    static jobject convert2JSatsCardStatus(JNIEnv *env, const SatscardStatus &status);

    static jobject convert2JUnspentOutput(JNIEnv *env, const UnspentOutput &output);

    static jobject convert2JUnspentOutputs(JNIEnv *env, const std::vector<UnspentOutput> &outputs);

    static jbyteArray convert2JByteArray(JNIEnv *env, const std::vector<unsigned char> &bytes);

    static jobject convert2JRecord(JNIEnv *env, const NDEFRecord &record);

    static jobjectArray convert2JRecords(JNIEnv *env, const std::vector<NDEFRecord> &records);

    static jobject convert2JPrimaryKeys(JNIEnv *env, const std::vector<PrimaryKey> &signers);

    static jobject convert2JPrimaryKey(JNIEnv *env, const PrimaryKey &key);

    static jobject convert2JBtcUri(JNIEnv *env, const BtcUri &btcUri);

    static jobject convert2JSignedMessage(JNIEnv *env, const std::string &address, const std::string &signature, const std::string &rfc2440);

    static jobject convert2JSignerTag(JNIEnv *env, const SignerTag &tag);

    static jobject convert2JSignerTags(JNIEnv *env, const std::vector<SignerTag> &tags);

    static jobject convert2JInt(JNIEnv *env, const int value);

    static jobject convert2JInts(JNIEnv *env, const std::vector<int> &tags);

    static jobject convert2JCoinTag(JNIEnv *env, const CoinTag &tag);

    static jobject convert2JCoinTags(JNIEnv *env, const std::vector<CoinTag> &tags);

    static jobject convert2JCoinCollection(JNIEnv *env, const CoinCollection &tag);

    static jobject convert2JCoinCollections(JNIEnv *env, const std::vector<CoinCollection> &tags);

    static jobject convert2JCoinStatus(JNIEnv *env, const CoinStatus &status);

    static jobject convert2JBSMSData(JNIEnv *env, const BSMSData &data);

    static jobject convert2JCollectionUnspentOutputs(JNIEnv *env, const std::vector<std::vector<UnspentOutput>> &outputs);
    static jobject convert2JPairAmount(JNIEnv *env, const std::pair<Amount, Amount> &pair);
    static jobject convert2JDraftRollOverTransaction(JNIEnv *env, const Transaction &transaction, const std::vector<int> &tagIds, const std::vector<int> &collectionIds);
    static jobjectArray convert2JDraftRollOverTransactions(JNIEnv *env, std::map<std::pair<std::set<int>, std::set<int>>, Transaction> txs);
    static jobject convert2JKeySetStatus(JNIEnv *env, const std::vector<KeysetStatus> &keySetStatus);
    static jobject convert2JGroupSandbox(JNIEnv *env, const GroupSandbox &groupSandbox);
    static jobject convert2JGroupsSandbox(JNIEnv *env, const std::vector<GroupSandbox> &groupSandbox);
    static jobject convert2JFreeGroupConfig(JNIEnv *env, const GroupConfig &config, const AddressType &addressType);
    static jobject convert2JGroupMessage(JNIEnv *env, const GroupMessage &message);
    static jobject convert2JGroupMessages(JNIEnv *env, const std::vector<GroupMessage> &messages);
    static jobject convert2JGroupWalletConfig(JNIEnv *env, const GroupWalletConfig &config);
    static jobject convert2JWalletTemplate(JNIEnv *env, const WalletTemplate &wallet_template);
    static jobject convert2JScriptNode(JNIEnv *env, const ScriptNode &node);
    static jobject convert2JScriptNodes(JNIEnv *env, const std::vector<ScriptNode> &nodes);
};
