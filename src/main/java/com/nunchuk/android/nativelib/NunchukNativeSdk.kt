package com.nunchuk.android.nativelib

import com.nunchuk.android.callbacks.SyncProgress
import com.nunchuk.android.exception.NCNativeException
import com.nunchuk.android.model.*
import com.nunchuk.android.model.bridge.toBridge
import com.nunchuk.android.type.AddressType
import com.nunchuk.android.type.ExportFormat
import com.nunchuk.android.type.WalletType

class NunchukNativeSdk {

    private val nunchukAndroid: LibNunchukAndroid = LibNunchukAndroid()

    @Throws(NCNativeException::class)
    fun initNunchuk(appSettings: AppSettings, passphrase: String, accountId: String) {
        nunchukAndroid.initNunchuk(
            chain = appSettings.chain.ordinal,
            hwiPath = appSettings.hwiPath,
            enableProxy = appSettings.enableProxy,
            testnetServers = appSettings.testnetServers,
            backendType = appSettings.backendType.ordinal,
            storagePath = appSettings.storagePath,
            passphrase = passphrase,
            accountId = accountId
        )
    }

    @Throws(NCNativeException::class)
    fun enableGenerateReceiveEvent() {
        nunchukAndroid.enableGenerateReceiveEvent()
    }

    @Throws(NCNativeException::class)
    fun createSigner(
        name: String,
        xpub: String,
        publicKey: String,
        derivationPath: String,
        masterFingerprint: String
    ) = nunchukAndroid.createSigner(
        name = name,
        xpub = xpub,
        publicKey = publicKey,
        derivationPath = derivationPath,
        masterFingerprint = masterFingerprint
    )

    @Throws(NCNativeException::class)
    fun createCoboSigner(
        name: String,
        jsonInfo: String
    ) = nunchukAndroid.createCoboSigner(
        name = name,
        jsonInfo = jsonInfo
    )

    @Throws(NCNativeException::class)
    fun createKeystoneSigner(
        qrData: String
    ) = nunchukAndroid.parseKeystoneSigner(
        qrData = qrData
    )

    @Throws(NCNativeException::class)
    fun createSoftwareSigner(
        name: String,
        mnemonic: String,
        passphrase: String
    ) = nunchukAndroid.createSoftwareSigner(
        name = name,
        mnemonic = mnemonic,
        passphrase = passphrase
    )

    @Throws(NCNativeException::class)
    fun getRemoteSigner(id: String) = nunchukAndroid.getRemoteSigners().first { it.masterSignerId == id }

    @Throws(NCNativeException::class)
    fun getRemoteSigners() = nunchukAndroid.getRemoteSigners()

    @Throws(NCNativeException::class)
    fun updateRemoteSigner(signer: SingleSigner) {
        nunchukAndroid.updateRemoteSigner(signer)
    }

    @Throws(NCNativeException::class)
    fun deleteRemoteSigner(masterFingerprint: String, derivationPath: String) {
        nunchukAndroid.deleteRemoteSigner(
            masterFingerprint = masterFingerprint,
            derivationPath = derivationPath
        )
    }

    @Throws(NCNativeException::class)
    fun createWallet(
        name: String,
        totalRequireSigns: Int,
        signers: List<SingleSigner>,
        addressType: AddressType,
        isEscrow: Boolean,
        description: String
    ) = nunchukAndroid.createWallet(
        name = name,
        totalRequireSigns = totalRequireSigns,
        signers = signers,
        addressType = addressType.ordinal,
        isEscrow = isEscrow,
        description = description
    )

    @Throws(NCNativeException::class)
    fun draftWallet(
        name: String,
        totalRequireSigns: Int,
        signers: List<SingleSigner>,
        addressType: AddressType,
        isEscrow: Boolean,
        description: String
    ) = nunchukAndroid.draftWallet(
        name = name,
        totalRequireSigns = totalRequireSigns,
        signers = signers,
        addressType = addressType.ordinal,
        isEscrow = isEscrow,
        description = description
    )

    @Throws(NCNativeException::class)
    fun getWallets() = nunchukAndroid.getWallets()

    @Throws(NCNativeException::class)
    fun exportWallet(walletId: String, filePath: String, format: ExportFormat) = nunchukAndroid.exportWallet(
        walletId = walletId,
        filePath = filePath,
        format = format.ordinal
    )

    @Throws(NCNativeException::class)
    fun exportCoboWallet(walletId: String) = nunchukAndroid.exportCoboWallet(
        walletId = walletId
    )

    @Throws(NCNativeException::class)
    fun exportKeystoneWallet(walletId: String) = nunchukAndroid.exportKeystoneWallet(
        walletId = walletId
    )

    @Throws(NCNativeException::class)
    fun getWallet(walletId: String) = nunchukAndroid.getWallet(walletId)

    @Throws(NCNativeException::class)
    fun updateWallet(wallet: Wallet) = nunchukAndroid.updateWallet(wallet.toBridge())

    @Throws(NCNativeException::class)
    fun generateMnemonic() = nunchukAndroid.generateMnemonic()

    //https://iancoleman.io/bip39/
    @Throws(NCNativeException::class)
    fun getBip39WordList() = nunchukAndroid.getBip39WordList()

    @Throws(NCNativeException::class)
    fun checkMnemonic(mnemonic: String) = nunchukAndroid.checkMnemonic(mnemonic)

    @Throws(NCNativeException::class)
    fun getMasterSigner(masterSignerId: String) = nunchukAndroid.getMasterSigner(masterSignerId)

    @Throws(NCNativeException::class)
    fun getMasterSigners() = nunchukAndroid.getMasterSigners()

    @Throws(NCNativeException::class)
    fun deleteMasterSigner(masterSignerId: String) = nunchukAndroid.deleteMasterSigner(masterSignerId)

    @Throws(NCNativeException::class)
    fun getSignersFromMasterSigner(masterSignerId: String) = nunchukAndroid.getSignersFromMasterSigner(masterSignerId)

    @Throws(NCNativeException::class)
    fun updateMasterSigner(masterSigner: MasterSigner) = nunchukAndroid.updateMasterSigner(masterSigner)

    @Throws(NCNativeException::class)
    fun getUnusedSignerFromMasterSigner(
        masterSignerId: String,
        walletType: WalletType,
        addressType: AddressType
    ) = nunchukAndroid.getUnusedSignerFromMasterSigner(
        masterSignerId = masterSignerId,
        walletType = walletType.ordinal,
        addressType = addressType.ordinal
    )

    @Throws(NCNativeException::class)
    fun broadcastTransaction(walletId: String, txId: String) = nunchukAndroid.broadcastTransaction(
        walletId = walletId,
        txId = txId
    )

    @Throws(NCNativeException::class)
    fun createTransactionUseCase(
        walletId: String,
        outputs: Map<String, Amount>,
        memo: String,
        inputs: List<UnspentOutput>,
        feeRate: Amount,
        subtractFeeFromAmount: Boolean
    ) = nunchukAndroid.createTransaction(
        walletId = walletId,
        outputs = outputs,
        memo = memo,
        inputs = inputs,
        feeRate = feeRate,
        subtractFeeFromAmount = subtractFeeFromAmount
    )

    @Throws(NCNativeException::class)
    fun draftTransaction(
        walletId: String,
        outputs: Map<String, Amount>,
        inputs: List<UnspentOutput>,
        feeRate: Amount,
        subtractFeeFromAmount: Boolean
    ) = nunchukAndroid.draftTransaction(
        walletId = walletId,
        outputs = outputs,
        inputs = inputs,
        feeRate = feeRate,
        subtractFeeFromAmount = subtractFeeFromAmount
    )

    @Throws(NCNativeException::class)
    fun deleteTransaction(walletId: String, txId: String) = nunchukAndroid.deleteTransaction(
        walletId = walletId,
        txId = txId
    )

    @Throws(NCNativeException::class)
    fun exportCoboTransaction(walletId: String, txId: String) = nunchukAndroid.exportCoboTransaction(
        walletId = walletId,
        txId = txId
    )

    @Throws(NCNativeException::class)
    fun exportKeystoneTransaction(walletId: String, txId: String) = nunchukAndroid.exportKeystoneTransaction(
        walletId = walletId,
        txId = txId
    )

    @Throws(NCNativeException::class)
    fun exportTransaction(
        walletId: String,
        txId: String,
        filePath: String
    ) = nunchukAndroid.exportTransaction(
        walletId = walletId,
        txId = txId,
        filePath = filePath
    )

    @Throws(NCNativeException::class)
    fun getTransaction(walletId: String, txId: String) = nunchukAndroid.getTransaction(
        walletId = walletId,
        txId = txId
    )

    @Throws(NCNativeException::class)
    fun replaceTransaction(
        walletId: String,
        txId: String,
        newFeeRate: Amount
    ) = nunchukAndroid.replaceTransaction(
        walletId = walletId,
        txId = txId,
        newFeeRate = newFeeRate
    )

    @Throws(NCNativeException::class)
    fun importTransaction(walletId: String, filePath: String) = nunchukAndroid.importTransaction(
        walletId = walletId,
        filePath = filePath
    )

    @Throws(NCNativeException::class)
    fun getTransactionHistory(walletId: String, count: Int, skip: Int) = nunchukAndroid.getTransactionHistory(
        walletId = walletId,
        count = count,
        skip = skip
    )

    @Throws(NCNativeException::class)
    fun importCoboTransaction(walletId: String, qrData: List<String>) = nunchukAndroid.importCoboTransaction(
        walletId = walletId,
        qrData = qrData
    )

    @Throws(NCNativeException::class)
    fun updateTransactionMemo(
        walletId: String,
        txId: String,
        newMemo: String
    ) = nunchukAndroid.updateTransactionMemo(
        walletId = walletId,
        txId = txId,
        newMemo = newMemo
    )

    @Throws(NCNativeException::class)
    fun signTransaction(
        walletId: String,
        txId: String,
        device: Device
    ) = nunchukAndroid.signTransaction(
        walletId = walletId,
        txId = txId,
        device = device
    )

    @Throws(NCNativeException::class)
    fun exportTransactionHistory(
        walletId: String,
        filePath: String,
        format: ExportFormat
    ) = nunchukAndroid.exportTransactionHistory(
        walletId = walletId,
        filePath = filePath,
        format = format.ordinal
    )

    @Throws(NCNativeException::class)
    fun getAddresses(
        walletId: String,
        used: Boolean,
        internal: Boolean
    ) = nunchukAndroid.getAddresses(
        walletId = walletId,
        used = used,
        internal = internal
    )

    @Throws(NCNativeException::class)
    fun getAddressBalance(walletId: String, address: String) = nunchukAndroid.getAddressBalance(
        walletId = walletId,
        address = address
    )

    @Throws(NCNativeException::class)
    fun getUnspentOutputs(walletId: String) = nunchukAndroid.getUnspentOutputs(
        walletId = walletId
    )

    @Throws(NCNativeException::class)
    fun newAddress(walletId: String, internal: Boolean = false) = nunchukAndroid.newAddress(
        walletId = walletId,
        internal = internal
    )

    @Throws(NCNativeException::class)
    fun valueFromAmount(amount: Amount) = nunchukAndroid.valueFromAmount(amount = amount)

    @Throws(NCNativeException::class)
    fun estimateFee(type: Int) = nunchukAndroid.estimateFee(type = type)

    @Throws(NCNativeException::class)
    fun isValidAddress(address: String) = nunchukAndroid.isValidAddress(address = address)

    @Throws(NCNativeException::class)
    fun getDevices() = nunchukAndroid.getDevices()

    @Throws(NCNativeException::class)
    fun getChainTip() = nunchukAndroid.getChainTip()

    @Throws(NCNativeException::class)
    fun sendSignerPassphrase(masterSignerId: String, passphrase: String) = nunchukAndroid.sendSignerPassphrase(masterSignerId, passphrase)

    @Throws(NCNativeException::class)
    fun deleteWallet(walletId: String) = nunchukAndroid.deleteWallet(walletId)

    @Throws(NCNativeException::class)
    fun initSharedWallet(
        roomId: String,
        name: String,
        totalSigns: Int,
        requireSigns: Int,
        addressType: AddressType,
        isEscrow: Boolean
    ) = nunchukAndroid.initSharedWallet(
        roomId = roomId,
        name = name,
        totalSigns = totalSigns,
        requireSigns = requireSigns,
        addressType = addressType.ordinal,
        isEscrow = isEscrow
    )

    @Throws(NCNativeException::class)
    fun joinSharedWallet(roomId: String, signer: SingleSigner) = nunchukAndroid.joinSharedWallet(
        roomId = roomId,
        signer = signer
    )

    @Throws(NCNativeException::class)
    fun leaveSharedWallet(roomId: String, joinEventId: String, reason: String) = nunchukAndroid.leaveSharedWallet(
        roomId = roomId,
        joinEventId = joinEventId,
        reason = reason
    )

    @Throws(NCNativeException::class)
    fun cancelSharedWallet(roomId: String, reason: String) = nunchukAndroid.cancelSharedWallet(
        roomId = roomId,
        reason = reason
    )

    @Throws(NCNativeException::class)
    fun createSharedWallet(roomId: String) = nunchukAndroid.createSharedWallet(roomId = roomId)

    @Throws(NCNativeException::class)
    fun consumeEvent(event: NunchukMatrixEvent) {
        nunchukAndroid.consumeEvent(event = event)
    }

    @Throws(NCNativeException::class)
    fun getRoomWallet(roomId: String) = nunchukAndroid.getRoomWallet(roomId = roomId)

    @Throws(NCNativeException::class)
    fun getAllRoomWallets() = nunchukAndroid.getAllRoomWallets()

    @Throws(NCNativeException::class)
    fun initRoomTransaction(
        roomId: String,
        outputs: Map<String, Amount>,
        memo: String,
        inputs: List<UnspentOutput>,
        feeRate: Amount,
        subtractFeeFromAmount: Boolean
    ) = nunchukAndroid.initRoomTransaction(roomId, outputs, memo, inputs, feeRate, subtractFeeFromAmount)

    @Throws(NCNativeException::class)
    fun signRoomTransaction(
        initEventId: String,
        device: Device
    ) = nunchukAndroid.signRoomTransaction(initEventId, device)

    @Throws(NCNativeException::class)
    fun rejectRoomTransaction(
        initEventId: String,
        reason: String
    ) = nunchukAndroid.rejectRoomTransaction(initEventId, reason)

    @Throws(NCNativeException::class)
    fun cancelRoomTransaction(
        initEventId: String,
        reason: String
    ) = nunchukAndroid.cancelRoomTransaction(initEventId, reason)

    @Throws(NCNativeException::class)
    fun broadcastRoomTransaction(
        initEventId: String
    ) = nunchukAndroid.broadcastRoomTransaction(initEventId)

    @Throws(NCNativeException::class)
    fun getRoomTransaction(
        initEventId: String
    ) = nunchukAndroid.getRoomTransaction(initEventId)

    @Throws(NCNativeException::class)
    fun getPendingTransactions(
        roomId: String
    ) = nunchukAndroid.getPendingTransactions(roomId)

    @Throws(NCNativeException::class)
    fun getTransactionId(
        eventId: String
    ) = nunchukAndroid.getTransactionId(eventId)

    @Throws(NCNativeException::class)
    fun consumeSyncEvent(event: NunchukMatrixEvent) {
        nunchukAndroid.consumeSyncEvent(
            event = event
        )
    }

    @Throws(NCNativeException::class)
    fun enableAutoBackUp(
        syncRoomId: String
    ) = nunchukAndroid.enableAutoBackUp(syncRoomId)

    @Throws(NCNativeException::class)
    fun backupFile(
        syncRoomId: String,
        fileJsonInfo: String,
        fileUrl: String
    ) = nunchukAndroid.backupFile(syncRoomId, fileJsonInfo, fileUrl)

    @Throws(NCNativeException::class)
    fun registerDownloadFileBackup(
    ) = nunchukAndroid.registerDownloadFileBackup()

    @Throws(NCNativeException::class)
    fun consumeSyncFile(
        fileJsonInfo: String,
        fileData: ByteArray
    ) = nunchukAndroid.consumeSyncFile(fileJsonInfo, fileData)
}