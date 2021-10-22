package com.nunchuk.android.nativelib

import com.nunchuk.android.exception.NCNativeException
import com.nunchuk.android.model.*
import com.nunchuk.android.model.bridge.WalletBridge

internal const val LIB_NAME = "nunchuk-android"

internal class LibNunchukAndroid {

    @Throws(NCNativeException::class)
    external fun initNunchuk(
        chain: Int,
        hwiPath: String,
        enableProxy: Boolean,
        testnetServers: List<String>,
        backendType: Int,
        storagePath: String,
        passphrase: String,
        accountId: String
    )

    @Throws(NCNativeException::class)
    external fun enableGenerateReceiveEvent()

    @Throws(NCNativeException::class)
    external fun createSigner(
        name: String,
        xpub: String,
        publicKey: String,
        derivationPath: String,
        masterFingerprint: String
    ): SingleSigner

    @Throws(NCNativeException::class)
    external fun createCoboSigner(
        name: String,
        jsonInfo: String
    ): SingleSigner

    @Throws(NCNativeException::class)
    external fun parseKeystoneSigner(
        qrData: String
    ): SingleSigner

    @Throws(NCNativeException::class)
    external fun createSoftwareSigner(
        name: String,
        mnemonic: String,
        passphrase: String = ""
    ): MasterSigner

    @Throws(NCNativeException::class)
    external fun getRemoteSigners(): List<SingleSigner>

    @Throws(NCNativeException::class)
    external fun createWallet(
        name: String,
        totalRequireSigns: Int,
        signers: List<SingleSigner>,
        addressType: Int,
        isEscrow: Boolean,
        description: String
    ): Wallet

    @Throws(NCNativeException::class)
    external fun getWallets(): List<Wallet>

    @Throws(NCNativeException::class)
    external fun deleteRemoteSigner(masterFingerprint: String, derivationPath: String)

    @Throws(NCNativeException::class)
    external fun updateRemoteSigner(signer: SingleSigner)

    @Throws(NCNativeException::class)
    external fun draftWallet(
        name: String,
        totalRequireSigns: Int,
        signers: List<SingleSigner>,
        addressType: Int,
        isEscrow: Boolean,
        description: String
    ): String

    @Throws(NCNativeException::class)
    external fun exportWallet(
        walletId: String,
        filePath: String,
        format: Int
    ): Boolean

    @Throws(NCNativeException::class)
    external fun importKeystoneWallet(
        qrData: String,
        description: String
    ): Wallet

    @Throws(NCNativeException::class)
    external fun getWallet(walletId: String): Wallet

    @Throws(NCNativeException::class)
    external fun updateWallet(wallet: WalletBridge): Boolean

    @Throws(NCNativeException::class)
    external fun exportCoboWallet(walletId: String): List<String>

    @Throws(NCNativeException::class)
    external fun exportKeystoneWallet(walletId: String): List<String>

    @Throws(NCNativeException::class)
    external fun generateMnemonic(): String

    @Throws(NCNativeException::class)
    external fun getBip39WordList(): List<String>

    @Throws(NCNativeException::class)
    external fun checkMnemonic(mnemonic: String): Boolean

    @Throws(NCNativeException::class)
    external fun getMasterSigner(masterSignerId: String): MasterSigner

    @Throws(NCNativeException::class)
    external fun getMasterSigners(): List<MasterSigner>

    @Throws(NCNativeException::class)
    external fun getSignersFromMasterSigner(masterSignerId: String): List<SingleSigner>

    @Throws(NCNativeException::class)
    external fun deleteMasterSigner(masterSignerId: String): Boolean

    @Throws(NCNativeException::class)
    external fun updateMasterSigner(masterSigner: MasterSigner): Boolean

    @Throws(NCNativeException::class)
    external fun getUnusedSignerFromMasterSigner(
        masterSignerId: String,
        walletType: Int,
        addressType: Int
    ): SingleSigner

    @Throws(NCNativeException::class)
    external fun broadcastTransaction(walletId: String, txId: String): Transaction

    @Throws(NCNativeException::class)
    external fun createTransaction(
        walletId: String,
        outputs: Map<String, Amount>,
        memo: String,
        inputs: List<UnspentOutput>,
        feeRate: Amount,
        subtractFeeFromAmount: Boolean
    ): Transaction

    @Throws(NCNativeException::class)
    external fun draftTransaction(
        walletId: String,
        outputs: Map<String, Amount>,
        inputs: List<UnspentOutput>,
        feeRate: Amount,
        subtractFeeFromAmount: Boolean
    ): Transaction

    @Throws(NCNativeException::class)
    external fun deleteTransaction(walletId: String, txId: String): Boolean

    @Throws(NCNativeException::class)
    external fun exportCoboTransaction(walletId: String, txId: String): List<String>

    @Throws(NCNativeException::class)
    external fun exportKeystoneTransaction(
        walletId: String,
        txId: String
    ): List<String>

    @Throws(NCNativeException::class)
    external fun exportTransaction(walletId: String, txId: String, filePath: String): Boolean

    @Throws(NCNativeException::class)
    external fun getTransaction(walletId: String, txId: String): Transaction

    @Throws(NCNativeException::class)
    external fun replaceTransaction(walletId: String, txId: String, newFeeRate: Amount): Transaction

    @Throws(NCNativeException::class)
    external fun importTransaction(walletId: String, filePath: String): Transaction

    @Throws(NCNativeException::class)
    external fun getTransactionHistory(walletId: String, count: Int, skip: Int): List<Transaction>

    @Throws(NCNativeException::class)
    external fun importCoboTransaction(walletId: String, qrData: List<String>): Transaction

    @Throws(NCNativeException::class)
    external fun updateTransactionMemo(walletId: String, txId: String, newMemo: String): Boolean

    @Throws(NCNativeException::class)
    external fun signTransaction(walletId: String, txId: String, device: Device): Transaction

    @Throws(NCNativeException::class)
    external fun exportTransactionHistory(walletId: String, filePath: String, format: Int)

    @Throws(NCNativeException::class)
    external fun getAddresses(walletId: String, used: Boolean, internal: Boolean): List<String>

    @Throws(NCNativeException::class)
    external fun getAddressBalance(walletId: String, address: String): Amount

    @Throws(NCNativeException::class)
    external fun getUnspentOutputs(walletId: String): List<UnspentOutput>

    @Throws(NCNativeException::class)
    external fun newAddress(walletId: String, internal: Boolean): String

    @Throws(NCNativeException::class)
    external fun valueFromAmount(amount: Amount): String

    @Throws(NCNativeException::class)
    external fun estimateFee(type: Int): Amount

    @Throws(NCNativeException::class)
    external fun isValidAddress(address: String): Boolean

    @Throws(NCNativeException::class)
    external fun getDevices(): List<Device>

    @Throws(NCNativeException::class)
    external fun getChainTip(): Int

    @Throws(NCNativeException::class)
    external fun sendSignerPassphrase(masterSignerId: String, passphrase: String)

    @Throws(NCNativeException::class)
    external fun deleteWallet(walletId: String): Boolean

    // SHARED WALLET
    @Throws(NCNativeException::class)
    external fun initSharedWallet(
        roomId: String,
        name: String,
        requireSigns: Int,
        totalSigns: Int,
        addressType: Int,
        isEscrow: Boolean
    ): NunchukMatrixEvent

    @Throws(NCNativeException::class)
    external fun joinSharedWallet(roomId: String, signer: SingleSigner): NunchukMatrixEvent

    @Throws(NCNativeException::class)
    external fun leaveSharedWallet(roomId: String, joinEventId: String, reason: String): NunchukMatrixEvent

    @Throws(NCNativeException::class)
    external fun cancelSharedWallet(roomId: String, reason: String): NunchukMatrixEvent

    @Throws(NCNativeException::class)
    external fun createSharedWallet(roomId: String): NunchukMatrixEvent

    @Throws(NCNativeException::class)
    external fun consumeEvent(event: NunchukMatrixEvent)

    @Throws(NCNativeException::class)
    external fun getRoomWallet(roomId: String): RoomWallet

    @Throws(NCNativeException::class)
    external fun getAllRoomWallets(): List<RoomWallet>

    @Throws(NCNativeException::class)
    external fun initRoomTransaction(
        roomId: String,
        outputs: Map<String, Amount>,
        memo: String,
        inputs: List<UnspentOutput>,
        feeRate: Amount,
        subtractFeeFromAmount: Boolean
    ): NunchukMatrixEvent

    @Throws(NCNativeException::class)
    external fun signRoomTransaction(
        initEventId: String,
        device: Device
    ): NunchukMatrixEvent

    @Throws(NCNativeException::class)
    external fun rejectRoomTransaction(
        initEventId: String,
        reason: String
    ): NunchukMatrixEvent

    @Throws(NCNativeException::class)
    external fun cancelRoomTransaction(
        initEventId: String,
        reason: String
    ): NunchukMatrixEvent

    @Throws(NCNativeException::class)
    external fun broadcastRoomTransaction(
        initEventId: String
    ): NunchukMatrixEvent

    @Throws(NCNativeException::class)
    external fun getRoomTransaction(
        initEventId: String
    ): RoomTransaction

    @Throws(NCNativeException::class)
    external fun getPendingTransactions(
        roomId: String
    ): List<RoomTransaction>

    @Throws(NCNativeException::class)
    external fun getTransactionId(
        eventId: String
    ): String

    companion object {
        init {
            System.loadLibrary(LIB_NAME)
        }
    }
}
