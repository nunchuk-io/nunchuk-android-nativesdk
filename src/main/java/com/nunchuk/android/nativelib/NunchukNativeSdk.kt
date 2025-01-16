/**************************************************************************
 * This file is part of the Nunchuk software (https://nunchuk.io/)        *							          *
 * Copyright (C) 2022 Nunchuk								              *
 *                                                                        *
 * This program is free software; you can redistribute it and/or          *
 * modify it under the terms of the GNU General Public License            *
 * as published by the Free Software Foundation; either version 3         *
 * of the License, or (at your option) any later version.                 *
 *                                                                        *
 * This program is distributed in the hope that it will be useful,        *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 * GNU General Public License for more details.                           *
 *                                                                        *
 * You should have received a copy of the GNU General Public License      *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
 *                                                                        *
 **************************************************************************/

package com.nunchuk.android.nativelib

import android.nfc.NdefRecord
import android.nfc.tech.IsoDep
import com.nunchuk.android.exception.NCNativeException
import com.nunchuk.android.model.*
import com.nunchuk.android.model.bridge.toBridge
import com.nunchuk.android.type.*

@Suppress("Unused")
class NunchukNativeSdk {

    private val nunchukAndroid: LibNunchukAndroid = LibNunchukAndroid()

    @Throws(NCNativeException::class)
    fun initNunchuk(
        appSettings: AppSettings,
        passphrase: String,
        accountId: String,
        deviceId: String,
        decoyPin: String,
        baseApiUrl: String,
    ) {
        nunchukAndroid.initNunchuk(
            chain = appSettings.chain.ordinal,
            hwiPath = appSettings.hwiPath,
            enableProxy = appSettings.enableProxy,
            testnetServers = appSettings.testnetServers,
            mainnetServers = appSettings.mainnetServers,
            signetServers = appSettings.signetServers,
            backendType = appSettings.backendType.ordinal,
            storagePath = appSettings.storagePath,
            passphrase = passphrase,
            accountId = accountId,
            deviceId = deviceId,
            decoyPin = decoyPin,
            baseApiUrl = baseApiUrl
        )
    }

    @Throws(NCNativeException::class)
    fun createSigner(
        name: String,
        xpub: String,
        publicKey: String,
        derivationPath: String,
        masterFingerprint: String,
        type: SignerType,
        tags: List<SignerTag>,
        replace: Boolean
    ) = nunchukAndroid.createSigner(
        name = name,
        xpub = xpub,
        publicKey = publicKey,
        derivationPath = derivationPath,
        masterFingerprint = masterFingerprint,
        type = type,
        tags = tags,
        replace = replace,
    )

    @Throws(NCNativeException::class)
    fun createKeystoneSigner(
        qrData: String,
    ) = nunchukAndroid.parseKeystoneSigner(
        qrData = qrData
    )

    @Throws(NCNativeException::class)
    fun parsePassportSigners(
        qrData: List<String>,
    ) = nunchukAndroid.parsePassportSigners(qrData)

    @Throws(NCNativeException::class)
    fun createSoftwareSigner(
        name: String,
        mnemonic: String,
        passphrase: String,
        isPrimary: Boolean,
        replace: Boolean,
    ) = nunchukAndroid.createSoftwareSigner(
        name = name,
        mnemonic = mnemonic,
        passphrase = passphrase,
        isPrimary = isPrimary,
        replace = replace
    )

    @Throws(NCNativeException::class)
    fun getRemoteSigner(masterFingerprint: String, derivationPath: String) =
        nunchukAndroid.getRemoteSigners()
            .first { it.masterFingerprint == masterFingerprint && it.derivationPath == derivationPath }

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
        description: String,
        decoyPin: String = ""
    ) = nunchukAndroid.createWallet(
        name = name,
        totalRequireSigns = totalRequireSigns,
        signers = signers,
        addressType = addressType.ordinal,
        isEscrow = isEscrow,
        description = description,
        decoyPin = decoyPin
    )

    @Throws(NCNativeException::class)
    fun draftWallet(
        name: String,
        totalRequireSigns: Int,
        signers: List<SingleSigner>,
        addressType: AddressType,
        isEscrow: Boolean,
        description: String,
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
    fun getMostRecentlyUsedWallets() = nunchukAndroid.getMostRecentlyUsedWallets()

    @Throws(NCNativeException::class)
    fun exportWallet(walletId: String, filePath: String, format: ExportFormat) =
        nunchukAndroid.exportWallet(
            walletId = walletId,
            filePath = filePath,
            format = format.ordinal
        )

    @Throws(NCNativeException::class)
    fun exportKeystoneWallet(walletId: String, density: Int) = nunchukAndroid.exportKeystoneWallet(
        walletId = walletId,
        density = density
    )

    @Throws(NCNativeException::class)
    fun exportBCR2020010Wallet(walletId: String, density: Int) =
        nunchukAndroid.exportBCR2020010Wallet(
            walletId = walletId,
            density = density
        )

    @Throws(NCNativeException::class)
    fun getWallet(walletId: String) = nunchukAndroid.getWallet(walletId)

    @Throws(NCNativeException::class)
    fun updateWallet(wallet: Wallet) = nunchukAndroid.updateWallet(wallet.toBridge())

    @Throws(NCNativeException::class)
    fun importKeystoneWallet(
        qrData: List<String>,
        description: String,
    ) = nunchukAndroid.importKeystoneWallet(
        qrData = qrData,
        description = description
    )

    @Throws(NCNativeException::class)
    fun generateMnemonic(count: Int = 24) = nunchukAndroid.generateMnemonic(count)

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
    fun deleteMasterSigner(masterSignerId: String) =
        nunchukAndroid.deleteMasterSigner(masterSignerId)

    @Throws(NCNativeException::class)
    fun getSignersFromMasterSigner(masterSignerId: String) =
        nunchukAndroid.getSignersFromMasterSigner(masterSignerId)

    @Throws(NCNativeException::class)
    fun updateMasterSigner(masterSigner: MasterSigner) =
        nunchukAndroid.updateMasterSigner(masterSigner)

    @Throws(NCNativeException::class)
    fun getUnusedSignerFromMasterSigner(
        masterSignerId: String,
        walletType: WalletType,
        addressType: AddressType,
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
    fun createTransaction(
        walletId: String,
        outputs: Map<String, Amount>,
        memo: String,
        inputs: List<TxInput>,
        feeRate: Amount,
        subtractFeeFromAmount: Boolean,
        replaceTxId: String,
    ) = nunchukAndroid.createTransaction(
        walletId = walletId,
        outputs = outputs,
        memo = memo,
        inputs = inputs,
        feeRate = feeRate,
        subtractFeeFromAmount = subtractFeeFromAmount,
        replaceTxId = replaceTxId
    )

    @Throws(NCNativeException::class)
    fun createInheritanceClaimTransaction(
        signers: List<SingleSigner>,
        psbt: String,
        subAmount: String,
        feeRate: String,
        fee: String,
        isDraft: Boolean,
    ) = nunchukAndroid.createInheritanceClaimTransaction(
        signers = signers,
        psbt = psbt,
        subAmount = subAmount,
        feeRate = feeRate,
        fee = fee,
        isDraft = isDraft
    )

    @Throws(NCNativeException::class)
    fun draftTransaction(
        walletId: String,
        outputs: Map<String, Amount>,
        inputs: List<TxInput>,
        feeRate: Amount,
        subtractFeeFromAmount: Boolean,
        replaceTxId: String,
    ) = nunchukAndroid.draftTransaction(
        walletId = walletId,
        outputs = outputs,
        inputs = inputs,
        feeRate = feeRate,
        subtractFeeFromAmount = subtractFeeFromAmount,
        replaceTxId
    )

    @Throws(NCNativeException::class)
    fun deleteTransaction(walletId: String, txId: String) = nunchukAndroid.deleteTransaction(
        walletId = walletId,
        txId = txId
    )

    @Throws(NCNativeException::class)
    fun exportKeystoneTransaction(walletId: String, txId: String, density: Int) =
        nunchukAndroid.exportKeystoneTransaction(
            walletId = walletId,
            txId = txId,
            density = density
        )

    @Throws(NCNativeException::class)
    fun exportTransaction(
        walletId: String,
        txId: String,
        filePath: String,
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
        newFeeRate: Amount,
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
    fun getTransactionHistory(walletId: String, count: Int, skip: Int) =
        nunchukAndroid.getTransactionHistory(
            walletId = walletId,
            count = count,
            skip = skip
        )

    @Throws(NCNativeException::class)
    fun importKeystoneTransaction(walletId: String, qrData: List<String>) =
        nunchukAndroid.importKeystoneTransaction(
            walletId = walletId,
            qrData = qrData
        )

    @Throws(NCNativeException::class)
    fun updateTransactionMemo(
        walletId: String,
        txId: String,
        newMemo: String,
    ) = nunchukAndroid.updateTransactionMemo(
        walletId = walletId,
        txId = txId,
        newMemo = newMemo
    )

    @Throws(NCNativeException::class)
    fun updateTransaction(
        walletId: String,
        txId: String,
        newTxId: String,
        rawTx: String,
        rejectMsg: String,
    ) = nunchukAndroid.updateTransaction(
        walletId = walletId,
        txId = txId,
        newTxId = newTxId,
        rawTx = rawTx,
        rejectMsg = rejectMsg
    )

    @Throws(NCNativeException::class)
    fun signTransaction(
        walletId: String,
        txId: String,
        device: Device,
    ) = nunchukAndroid.signTransaction(
        walletId = walletId,
        txId = txId,
        device = device
    )

    @Throws(NCNativeException::class)
    fun signAirgapTransaction(initEventId: String, masterSignerId: String) =
        nunchukAndroid.signAirgapTransaction(initEventId, masterSignerId)

    @Throws(NCNativeException::class)
    fun exportTransactionHistory(
        walletId: String,
        filePath: String,
        format: ExportFormat,
    ) = nunchukAndroid.exportTransactionHistory(
        walletId = walletId,
        filePath = filePath,
        format = format.ordinal
    )

    @Throws(NCNativeException::class)
    fun getAddresses(
        walletId: String,
        used: Boolean,
        internal: Boolean,
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
    fun sendSignerPassphrase(masterSignerId: String, passphrase: String) =
        nunchukAndroid.sendSignerPassphrase(masterSignerId, passphrase)

    @Throws(NCNativeException::class)
    fun clearSignerPassphrase(masterSignerId: String) =
        nunchukAndroid.clearSignerPassphrase(masterSignerId)

    @Throws(NCNativeException::class)
    fun deleteWallet(walletId: String) = nunchukAndroid.deleteWallet(walletId)

    @Throws(NCNativeException::class)
    fun initSharedWallet(
        roomId: String,
        name: String,
        totalSigns: Int,
        requireSigns: Int,
        addressType: AddressType,
        isEscrow: Boolean,
        des: String,
        signers: List<SingleSigner>,
    ) = nunchukAndroid.initSharedWallet(
        roomId = roomId,
        name = name,
        totalSigns = totalSigns,
        requireSigns = requireSigns,
        addressType = addressType.ordinal,
        isEscrow = isEscrow,
        des = des,
        signers = signers
    )

    @Throws(NCNativeException::class)
    fun joinSharedWallet(roomId: String, signer: SingleSigner) = nunchukAndroid.joinSharedWallet(
        roomId = roomId,
        signer = signer
    )

    @Throws(NCNativeException::class)
    fun leaveSharedWallet(roomId: String, joinEventId: String, reason: String) =
        nunchukAndroid.leaveSharedWallet(
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
    fun hasRoomWallet(roomId: String) = nunchukAndroid.hasRoomWallet(roomId = roomId)

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
        subtractFeeFromAmount: Boolean,
    ) = nunchukAndroid.initRoomTransaction(
        roomId = roomId,
        outputs = outputs,
        memo = memo,
        inputs = inputs,
        feeRate = feeRate,
        subtractFeeFromAmount = subtractFeeFromAmount
    )

    @Throws(NCNativeException::class)
    fun signRoomTransaction(
        initEventId: String,
        device: Device,
    ) = nunchukAndroid.signRoomTransaction(initEventId, device)

    @Throws(NCNativeException::class)
    fun rejectRoomTransaction(
        initEventId: String,
        reason: String,
    ) = nunchukAndroid.rejectRoomTransaction(initEventId, reason)

    @Throws(NCNativeException::class)
    fun cancelRoomTransaction(
        initEventId: String,
        reason: String,
    ) = nunchukAndroid.cancelRoomTransaction(initEventId, reason)

    @Throws(NCNativeException::class)
    fun broadcastRoomTransaction(
        initEventId: String,
    ) = nunchukAndroid.broadcastRoomTransaction(initEventId)

    @Throws(NCNativeException::class)
    fun getRoomTransaction(
        initEventId: String,
    ) = nunchukAndroid.getRoomTransaction(initEventId)

    @Throws(NCNativeException::class)
    fun getPendingTransactions(
        roomId: String,
    ) = nunchukAndroid.getPendingTransactions(roomId)

    @Throws(NCNativeException::class)
    fun getTransactionId(
        eventId: String,
    ) = nunchukAndroid.getTransactionId(eventId)

    @Throws(NCNativeException::class)
    fun consumeSyncEvent(event: NunchukMatrixEvent) {
        nunchukAndroid.consumeSyncEvent(event = event)
    }

    @Throws(NCNativeException::class)
    fun enableAutoBackUp(
        enable: Boolean,
    ) = nunchukAndroid.enableAutoBackUp(enable)

    @Throws(NCNativeException::class)
    fun registerAutoBackUp(
        syncRoomId: String,
        accessToken: String,
    ) = nunchukAndroid.registerAutoBackUp(syncRoomId, accessToken)

    @Throws(NCNativeException::class)
    fun uploadFileCallback(
        fileJsonInfo: String,
        fileUrl: String,
    ) = nunchukAndroid.uploadFileCallback(fileJsonInfo = fileJsonInfo, fileUrl = fileUrl)

    @Throws(NCNativeException::class)
    fun registerDownloadFileBackup() = nunchukAndroid.registerDownloadFileBackup()

    @Throws(NCNativeException::class)
    fun downloadFileCallback(
        fileJsonInfo: String,
        fileData: ByteArray,
    ) = nunchukAndroid.downloadFileCallback(fileJsonInfo, fileData)

    @Throws(NCNativeException::class)
    fun writeFileCallback(
        fileJsonInfo: String,
        fileData: String,
    ) = nunchukAndroid.writeFileCallback(fileJsonInfo, fileData)

    @Throws(NCNativeException::class)
    fun importWallet(
        filePath: String,
        name: String,
        description: String,
    ) = nunchukAndroid.importWallet(filePath = filePath, name = name, description = description)

    @Throws(NCNativeException::class)
    fun healthCheckMasterSigner(
        fingerprint: String,
        message: String,
        signature: String,
        path: String,
    ) = nunchukAndroid.healthCheckMasterSigner(
        fingerprint = fingerprint,
        message = message,
        signature = signature,
        path = path
    )

    @Throws(NCNativeException::class)
    fun sendErrorEvent(
        roomId: String,
        platform: String,
        code: String,
        message: String,
    ) = nunchukAndroid.sendErrorEvent(
        roomId = roomId,
        platform = platform,
        code = code,
        message = message
    )

    @Throws(NCNativeException::class)
    fun backup() = nunchukAndroid.backup()

    @Throws(NCNativeException::class)
    fun tapSignerStatus(
        isoDep: IsoDep,
    ) = nunchukAndroid.tapSignerStatus(isoDep)

    @Throws(NCNativeException::class)
    fun initTapSigner(
        isoDep: IsoDep,
        oldCvc: String,
        chainCode: String,
    ) = nunchukAndroid.initTapSigner(isoDep, oldCvc, chainCode)

    @Throws(NCNativeException::class)
    fun createTapSigner(
        isoDep: IsoDep,
        cvc: String,
        name: String,
        replace: Boolean
    ) = nunchukAndroid.createTapSigner(isoDep, cvc, name, replace)

    @Throws(NCNativeException::class)
    fun signTransactionByTapSigner(
        isoDep: IsoDep,
        cvc: String,
        walletId: String,
        txId: String,
    ) = nunchukAndroid.signTransactionByTapSigner(isoDep, cvc, walletId, txId)

    @Throws(NCNativeException::class)
    fun getBackupTapSignerKey(
        isoDep: IsoDep,
        cvc: String,
        masterSignerId: String,
    ) = nunchukAndroid.getBackupTapSignerKey(isoDep, cvc, masterSignerId)

    @Throws(NCNativeException::class)
    fun changeCvcTapSigner(
        isoDep: IsoDep,
        oldCvc: String,
        newCvc: String,
        masterSignerId: String,
    ) = nunchukAndroid.changeCvcTapSigner(isoDep, oldCvc, newCvc, masterSignerId)

    @Throws(NCNativeException::class)
    fun healthCheckTapSigner(
        isoDep: IsoDep,
        cvc: String,
        fingerprint: String,
        message: String,
        signature: String,
        path: String,
    ) = nunchukAndroid.healthCheckTapSigner(isoDep, cvc, fingerprint, message, signature, path)

    @Throws(NCNativeException::class)
    fun waitTapSigner(
        isoDep: IsoDep,
    ) = nunchukAndroid.waitTapSigner(isoDep)

    @Throws(NCNativeException::class)
    fun generateRandomChainCode(): String = nunchukAndroid.generateRandomChainCode()

    @Throws(NCNativeException::class)
    fun tapSignerTopUpXpub(
        isoDep: IsoDep,
        cvc: String,
        masterSignerId: String,
    ) = nunchukAndroid.tapSignerTopUpXpub(isoDep, cvc, masterSignerId)

    @Throws(NCNativeException::class)
    fun cacheDefaultTapsignerMasterSignerXPub(
        isoDep: IsoDep,
        cvc: String,
        masterSignerId: String,
    ) = nunchukAndroid.cacheDefaultTapsignerMasterSignerXPub(isoDep, cvc, masterSignerId)

    @Throws(NCNativeException::class)
    fun getTapSignerStatusFromMasterSigner(
        masterSignerId: String,
    ) = nunchukAndroid.getTapSignerStatusFromMasterSigner(masterSignerId)

    @Throws(NCNativeException::class)
    fun signRoomTransactionByTapSigner(
        isoDep: IsoDep,
        cvc: String,
        initEventId: String,
    ) = nunchukAndroid.signRoomTransactionByTapSigner(isoDep, cvc, initEventId)

    @Throws(NCNativeException::class)
    fun decryptBackUpKey(
        backUpKey: String,
        decryptionKey: String,
        rawName: String,
    ) = nunchukAndroid.decryptBackUpKey(backUpKey, decryptionKey, rawName)

    @Throws(NCNativeException::class)
    fun parseWalletDescriptor(content: String) = nunchukAndroid.parseWalletDescriptor(content)

    fun parseKeystoneWallet(chain: Int, qrData: List<String>) =
        nunchukAndroid.parseKeystoneWallet(chain, qrData)

    @Throws(NCNativeException::class)
    fun hasSigner(signer: SingleSigner) = nunchukAndroid.hasSigner(signer)

    @Throws(NCNativeException::class)
    fun getSigner(signer: SingleSigner) = nunchukAndroid.getSigner(signer)

    @Throws(NCNativeException::class)
    fun getMatrixEvent(
        eventId: String,
    ) = nunchukAndroid.getMatrixEvent(eventId)

    @Throws(NCNativeException::class)
    fun getSatsCardStatus(
        isoDep: IsoDep,
    ) = nunchukAndroid.getSatsCardStatus(isoDep)

    @Throws(NCNativeException::class)
    fun setupSatsCard(
        isoDep: IsoDep,
        cvc: String,
        chainCode: String,
    ) = nunchukAndroid.setupSatsCard(isoDep, cvc, chainCode)

    @Throws(NCNativeException::class)
    fun loadSatsCardSlotsBalance(
        slots: List<SatsCardSlot>,
    ) = nunchukAndroid.loadSatsCardSlotsBalance(slots)

    @Throws(NCNativeException::class)
    fun unsealSatsCard(
        isoDep: IsoDep,
        cvc: String,
        slot: SatsCardSlot,
    ) = nunchukAndroid.unsealSatsCard(isoDep, cvc, slot)

    @Throws(NCNativeException::class)
    fun sweepBalance(
        slots: List<SatsCardSlot>,
        walletId: String,
        feeRate: Int,
    ) = nunchukAndroid.sweepBalance(slots, walletId, feeRate)

    @Throws(NCNativeException::class)
    fun getSlotKeys(
        isoDep: IsoDep,
        cvc: String,
        slots: List<SatsCardSlot>,
    ) = nunchukAndroid.getSlotKeys(isoDep, cvc, slots)

    @Throws(NCNativeException::class)
    fun getAutoCardStatus(
        isoDep: IsoDep,
    ) = nunchukAndroid.getAutoCardStatus(isoDep)

    @Throws(NCNativeException::class)
    fun draftSatscardTransaction(
        slots: List<SatsCardSlot>,
        address: String,
        feeRate: Int,
    ) = nunchukAndroid.draftSatscardTransaction(slots, address, feeRate)

    @Throws(NCNativeException::class)
    fun waitAutoCard(
        isoDep: IsoDep,
    ) = nunchukAndroid.waitAutoCard(isoDep)

    @Throws(NCNativeException::class)
    fun loadTransaction(
        txId: String,
    ) = nunchukAndroid.loadTransaction(txId)

    @Throws(NCNativeException::class)
    fun getMk4Signers(
        records: Array<NdefRecord>,
    ) = nunchukAndroid.getMk4Signers(records)

    @Throws(NCNativeException::class)
    fun createMk4Signer(
        signer: SingleSigner,
    ) = nunchukAndroid.createMk4Signer(signer)

    @Throws(NCNativeException::class)
    fun exportWalletToMk4(
        walletId: String,
    ) = nunchukAndroid.exportWalletToMk4(walletId)

    @Throws(NCNativeException::class)
    fun exportWalletToBsmsById(
        walletId: String,
    ) = nunchukAndroid.exportWalletToBsmsById(walletId)

    @Throws(NCNativeException::class)
    fun exportWalletToBsms(
        wallet: Wallet,
    ) = nunchukAndroid.exportWalletToBsms(wallet.toBridge())

    @Throws(NCNativeException::class)
    fun exportPsbtToMk4(
        walletId: String,
        txId: String,
    ) = nunchukAndroid.exportPsbtToMk4(walletId, txId)

    @Throws(NCNativeException::class)
    fun exportRawPsbtToMk4(
        psbt: String,
    ) = nunchukAndroid.exportRawPsbtToMk4(psbt)

    @Throws(NCNativeException::class)
    fun importTransactionFromMk4(
        walletId: String,
        records: Array<NdefRecord>,
    ) = nunchukAndroid.importTransactionFromMk4(walletId, records)

    fun parseWalletFromMk4(
        chain: Int,
        records: Array<NdefRecord>,
    ) = nunchukAndroid.parseWalletFromMk4(chain, records)

    @Throws(NCNativeException::class)
    fun importWalletFromMk4(
        chain: Int,
        records: Array<NdefRecord>,
    ) = nunchukAndroid.importWalletFromMk4(chain, records)

    @Throws(NCNativeException::class)
    fun getPrimaryKeyAddress(mnemonic: String, passphrase: String) =
        nunchukAndroid.getPrimaryKeyAddress(
            mnemonic = mnemonic,
            passphrase = passphrase
        )

    @Throws(NCNativeException::class)
    fun signLoginMessage(mnemonic: String, passphrase: String, message: String) =
        nunchukAndroid.signLoginMessage(
            mnemonic = mnemonic,
            passphrase = passphrase,
            message = message
        )

    @Throws(NCNativeException::class)
    fun signLoginMessageImpl(masterSignerId: String, message: String) =
        nunchukAndroid.signLoginMessageImpl(
            masterSignerId = masterSignerId,
            message = message
        )

    @Throws(NCNativeException::class)
    fun getMasterFingerprint(mnemonic: String, passphrase: String) =
        nunchukAndroid.getMasterFingerprint(
            mnemonic = mnemonic,
            passphrase = passphrase,
        )

    @Throws(NCNativeException::class)
    fun getPrimaryKeys(chain: Int, storagePath: String) =
        nunchukAndroid.getPrimaryKeys(
            chain = chain,
            storagePath = storagePath,
        )

    @Throws(NCNativeException::class)
    fun parseBtcUri(
        uri: String,
    ) = nunchukAndroid.parseBtcUri(uri)

    @Throws(NCNativeException::class)
    fun setSelectedWallet(walletId: String) = nunchukAndroid.setSelectedWallet(walletId)

    @Throws(NCNativeException::class)
    fun getColdCardWallets(records: Array<NdefRecord>) = nunchukAndroid.getColdCardWallets(records)

    @Throws(NCNativeException::class)
    fun createWallet2(wallet: Wallet): Wallet = nunchukAndroid.createWallet2(wallet.toBridge())

    @Throws(NCNativeException::class)
    fun deletePrimaryKey(): Boolean = nunchukAndroid.deletePrimaryKey()

    @Throws(NCNativeException::class)
    fun generateColdCardHealthCheckMessage(
        derivationPath: String,
        message: String? = null,
    ): Array<NdefRecord> =
        nunchukAndroid.generateColdCardHealthCheckMessage(derivationPath, message)

    @Throws(NCNativeException::class)
    fun healthCheckColdCard(signer: SingleSigner, records: Array<NdefRecord>): ColdCardHealth =
        nunchukAndroid.healthCheckColdCard(signer, records)

    @Throws(NCNativeException::class)
    fun isValidDerivationPath(path: String): Boolean = nunchukAndroid.isValidDerivationPath(path)

    @Throws(NCNativeException::class)
    fun getSignerFromMasterSigner(masterSignerId: String, path: String): SingleSigner =
        nunchukAndroid.getSignerFromMasterSigner(masterSignerId, path)

    @Throws(NCNativeException::class)
    fun getSignerFromMasterSigner(
        masterSignerId: String,
        walletType: Int,
        addressType: Int,
        index: Int,
    ) = nunchukAndroid.getSignerFromMasterSignerByIndex(
        masterSignerId,
        walletType,
        addressType,
        index
    )

    @Throws(NCNativeException::class)
    fun getSignerFromTapsignerMasterSigner(
        isoDep: IsoDep,
        cvc: String,
        masterSignerId: String,
        walletType: Int,
        addressType: Int,
        index: Int,
    ) = nunchukAndroid.getSignerFromTapsignerMasterSigner(
        isoDep = isoDep,
        cvc = cvc,
        masterSignerId = masterSignerId,
        walletType = walletType,
        addressType = addressType,
        index = index
    )

    @Throws(NCNativeException::class)
    fun getDefaultSignerFromMasterSigner(
        masterSignerId: String,
        walletType: Int,
        addressType: Int,
    ): SingleSigner =
        nunchukAndroid.getDefaultSignerFromMasterSigner(masterSignerId, walletType, addressType)

    @Throws(NCNativeException::class)
    fun parseJsonSigners(
        str: String,
        type: SignerType,
    ): List<SingleSigner> = nunchukAndroid.parseJsonSigners(str, type)

    @Throws(NCNativeException::class)
    fun verifyTapSignerBackup(
        backUpKey: String,
        decryptionKey: String,
        masterSignerId: String,
    ): Boolean = nunchukAndroid.verifyTapSignerBackup(backUpKey, decryptionKey, masterSignerId)

    @Throws(NCNativeException::class)
    fun verifyTapSignerBackupContent(
        content: ByteArray,
        backUpKey: String,
        masterSignerId: String,
    ): Boolean = nunchukAndroid.verifyTapSignerBackupContent(content, backUpKey, masterSignerId)

    @Throws(NCNativeException::class)
    fun verifyColdCardBackup(
        data: ByteArray,
        backUpKey: String,
        masterSignerId: String,
    ) = nunchukAndroid.verifyColdCardBackup(data, backUpKey, masterSignerId)

    @Throws(NCNativeException::class)
    fun importTapsignerMasterSignerContent(
        content: ByteArray,
        backUpKey: String,
        rawName: String,
    ): MasterSigner = nunchukAndroid.importTapsignerMasterSignerContent(content, backUpKey, rawName)

    @Throws(NCNativeException::class)
    fun importBackupKeyContent(
        content: ByteArray,
        backUpKey: String,
        rawName: String,
    ): MasterSigner = nunchukAndroid.importBackupKeyContent(content, backUpKey, rawName)

    @Throws(NCNativeException::class)
    fun hasWallet(
        walletId: String,
    ) = nunchukAndroid.hasWallet(walletId)

    @Throws(NCNativeException::class)
    fun addTapSigner(
        cardId: String,
        xfp: String,
        name: String,
        version: String,
        brithHeight: Int,
        isTestNet: Boolean,
        replace: Boolean
    ) = nunchukAndroid.addTapSigner(cardId, xfp, name, version, brithHeight, isTestNet, replace)

    @Throws(NCNativeException::class)
    fun signerTypeFromStr(signerType: String): SignerType =
        nunchukAndroid.signerTypeFromStr(signerType)

    @Throws(NCNativeException::class)
    fun importPsbt(walletId: String, psbt: String): Transaction =
        nunchukAndroid.importPsbt(walletId, psbt)

    @Throws(NCNativeException::class)
    fun signHealthCheckMessage(signer: SingleSigner, messagesToSign: String) =
        nunchukAndroid.signHealthCheckMessage(signer, messagesToSign)

    @Throws(NCNativeException::class)
    fun signHealthCheckMessageTapSigner(
        isoDep: IsoDep,
        cvc: String,
        signer: SingleSigner,
        messagesToSign: String,
    ) = nunchukAndroid.signHealthCheckMessageTapSigner(isoDep, cvc, signer, messagesToSign)

    @Throws(NCNativeException::class)
    fun signHealthCheckMessageTapSignerSignIn(
        signer: SingleSigner,
        isoDep: IsoDep,
        cvc: String,
        messagesToSign: String,
    ) = nunchukAndroid.signHealthCheckMessageTapSignerSignIn(signer, isoDep, cvc, messagesToSign)

    @Throws(NCNativeException::class)
    fun signMessageColdCard(derivationPath: String, messagesToSign: String) =
        nunchukAndroid.signMessageColdCard(derivationPath, messagesToSign)

    @Throws(NCNativeException::class)
    fun createRequestToken(signature: String, fingerprint: String) =
        nunchukAndroid.createRequestToken(signature, fingerprint)

    @Throws(NCNativeException::class)
    fun getHealthCheckMessage(body: String): String = nunchukAndroid.getHealthCheckMessage(body)

    @Throws(NCNativeException::class)
    fun getHealthCheckDummyTxMessage(walletId: String, body: String) =
        nunchukAndroid.getHealthCheckDummyTxMessage(walletId, body)

    @Throws(NCNativeException::class)
    fun getDummyTx(walletId: String, message: String) = nunchukAndroid.getDummyTx(walletId, message)

    @Throws(NCNativeException::class)
    fun getSignInDummyTx(message: String) = nunchukAndroid.getSignInDummyTx(message)

    @Throws(NCNativeException::class)
    fun getDummyTxByByteArray(walletId: String, fileData: ByteArray) =
        nunchukAndroid.getDummyTxByByteArray(walletId, fileData)

    @Throws(NCNativeException::class)
    fun getSignInDummyTxByByteArray(fileData: ByteArray) =
        nunchukAndroid.getSignInDummyTxByByteArray(fileData)

    @Throws(NCNativeException::class)
    fun exportKeystoneDummyTransaction(txToSign: String, density: Int) =
        nunchukAndroid.exportKeystoneDummyTransaction(txToSign, density)

    @Throws(NCNativeException::class)
    fun parseKeystoneDummyTransaction(qrs: List<String>) =
        nunchukAndroid.parseKeystoneDummyTransaction(qrs)

    @Throws(NCNativeException::class)
    fun getDummyTransactionSignature(signer: SingleSigner, psbt: String) =
        nunchukAndroid.getDummyTransactionSignature(signer, psbt)

    @Throws(NCNativeException::class)
    fun getColdcardSignatureFromPsbt(signer: SingleSigner, records: Array<NdefRecord>) =
        nunchukAndroid.getColdcardSignatureFromPsbt(signer, records)

    fun updateTransactionSchedule(walletId: String, txId: String, broadcastTime: Long) =
        nunchukAndroid.updateTransactionSchedule(walletId, txId, broadcastTime)

    @Throws(NCNativeException::class)
    fun forceRefreshWallet(walletId: String) =
        nunchukAndroid.forceRefreshWallet(walletId)

    @Throws(NCNativeException::class)
    fun analyzeQr(qrs: List<String>): Double = nunchukAndroid.analyzeQr(qrs)

    @Throws(NCNativeException::class)
    fun hashSHA256(data: String): String = nunchukAndroid.hashSHA256(data)

    @Throws(NCNativeException::class)
    fun lockCoin(walletId: String, txId: String, vout: Int): Boolean =
        nunchukAndroid.lockCoin(walletId, txId, vout)

    fun unlockCoin(walletId: String, txId: String, vout: Int): Boolean =
        nunchukAndroid.unlockCoin(walletId, txId, vout)

    @Throws(NCNativeException::class)
    fun addToCoinTag(walletId: String, txId: String, tagId: Int, vout: Int): Boolean =
        nunchukAndroid.addToCoinTag(walletId, txId, tagId, vout)

    @Throws(NCNativeException::class)
    fun removeFromCoinTag(walletId: String, txId: String, tagId: Int, vout: Int): Boolean =
        nunchukAndroid.removeFromCoinTag(walletId, txId, tagId, vout)

    @Throws(NCNativeException::class)
    fun addToCoinCollection(
        walletId: String,
        txId: String,
        collectionId: Int,
        vout: Int,
    ): Boolean = nunchukAndroid.addToCoinCollection(walletId, txId, collectionId, vout)

    @Throws(NCNativeException::class)
    fun removeFromCoinCollection(
        walletId: String,
        txId: String,
        collectionId: Int,
        vout: Int,
    ): Boolean = nunchukAndroid.removeFromCoinCollection(walletId, txId, collectionId, vout)

    @Throws(NCNativeException::class)
    fun getCoinByTag(walletId: String, tagId: Int): List<UnspentOutput> =
        nunchukAndroid.getCoinByTag(walletId, tagId)

    @Throws(NCNativeException::class)
    fun getCoinInCollection(walletId: String, collectionId: Int): List<UnspentOutput> =
        nunchukAndroid.getCoinInCollection(walletId, collectionId)

    @Throws(NCNativeException::class)
    fun getCoinTags(walletId: String): List<CoinTag> = nunchukAndroid.getCoinTags(walletId)

    @Throws(NCNativeException::class)
    fun createCoinTag(walletId: String, name: String, color: String): CoinTag =
        nunchukAndroid.createCoinTag(walletId, name, color)

    @Throws(NCNativeException::class)
    fun updateCoinTag(walletId: String, coinTag: CoinTag): Boolean =
        nunchukAndroid.updateCoinTag(walletId, coinTag)

    @Throws(NCNativeException::class)
    fun deleteCoinTag(walletId: String, tagId: Int): Boolean =
        nunchukAndroid.deleteCoinTag(walletId, tagId)

    @Throws(NCNativeException::class)
    fun getCoinCollections(walletId: String): List<CoinCollection> =
        nunchukAndroid.getCoinCollections(walletId)

    @Throws(NCNativeException::class)
    fun createCoinCollection(walletId: String, name: String): CoinCollection =
        nunchukAndroid.createCoinCollection(walletId, name)

    @Throws(NCNativeException::class)
    fun updateCoinCollection(
        walletId: String,
        coinCollection: CoinCollection,
        applyToExistingCoins: Boolean
    ): Boolean =
        nunchukAndroid.updateCoinCollection(walletId, coinCollection, applyToExistingCoins)

    @Throws(NCNativeException::class)
    fun deleteCoinCollection(walletId: String, collectionId: Int): Boolean =
        nunchukAndroid.deleteCoinCollection(walletId, collectionId)

    @Throws(NCNativeException::class)
    fun isMyAddress(walletId: String, address: String): Boolean =
        nunchukAndroid.isMyAddress(walletId, address)

    @Throws(NCNativeException::class)
    fun exportCoinControlData(walletId: String): String =
        nunchukAndroid.exportCoinControlData(walletId)

    @Throws(NCNativeException::class)
    fun importCoinControlData(walletId: String, data: String, force: Boolean) =
        nunchukAndroid.importCoinControlData(walletId, data, force)

    @Throws(NCNativeException::class)
    fun exportCoinControlBIP329(walletId: String): String =
        nunchukAndroid.exportCoinControlBIP329(walletId)

    @Throws(NCNativeException::class)
    fun importCoinControlBIP329(walletId: String, data: String) =
        nunchukAndroid.importCoinControlBIP329(walletId, data)

    @Throws(NCNativeException::class)
    fun getCoinAncestry(walletId: String, txId: String, vout: Int) =
        nunchukAndroid.getCoinAncestry(walletId, txId, vout)

    @Throws(NCNativeException::class)
    fun signMessageByTapSigner(
        isoDep: IsoDep,
        cvc: String,
        message: String,
        path: String,
        masterSignerId: String,
    ) = nunchukAndroid.signMessageByTapSigner(isoDep, cvc, message, path, masterSignerId)

    @Throws(NCNativeException::class)
    fun signMessageBySoftwareKey(
        message: String,
        path: String,
        masterSignerId: String,
    ) = nunchukAndroid.signMessageBySoftwareKey(message, path, masterSignerId)

    @Throws(NCNativeException::class)
    fun getHealthCheckPath() = nunchukAndroid.getHealthCheckPath()

    @Throws(NCNativeException::class)
    fun getRawTransaction(walletId: String, txId: String) =
        nunchukAndroid.getRawTransaction(walletId, txId)

    @Throws(NCNativeException::class)
    fun importDummyTx(dummyTransactionJson: String): Transaction? =
        nunchukAndroid.importDummyTx(dummyTransactionJson)

    @Throws(NCNativeException::class)
    fun deleteDummyTx(walletId: String, txId: String): Boolean =
        nunchukAndroid.deleteDummyTx(walletId, txId)

    @Throws(NCNativeException::class)
    fun saveDummyTxRequestToken(
        walletId: String,
        txId: String,
        requestToken: String,
    ) = nunchukAndroid.saveDummyTxRequestToken(walletId, txId, requestToken)

    @Throws(NCNativeException::class)
    fun getDummyTxRequestToken(
        walletId: String,
        txId: String,
    ) = nunchukAndroid.getDummyTxRequestToken(walletId, txId)

    @Throws(NCNativeException::class)
    fun getDummyTransactions(walletId: String): List<Transaction> =
        nunchukAndroid.getDummyTransactions(walletId)

    @Throws(NCNativeException::class)
    fun getDummyTxByTxId(walletId: String, txId: String): Transaction? =
        nunchukAndroid.getDummyTransaction(walletId, txId)

    @Throws
    fun getCurrentSignerIndex(
        xfp: String,
        walletType: Int,
        addressType: Int,
    ) = nunchukAndroid.getCurrentSignerIndex(xfp, walletType, addressType)

    @Throws
    fun getSignerByIndex(
        xfp: String,
        walletType: Int,
        addressType: Int,
        index: Int,
    ): SingleSigner? = nunchukAndroid.getSignerByIndex(xfp, walletType, addressType, index)

    @kotlin.jvm.Throws
    fun getAddressByIndex(
        wallet: Wallet,
        from: Int,
        to: Int,
    ): List<String> = nunchukAndroid.getAddressByIndex(wallet.toBridge(), from, to)

    @Throws(NCNativeException::class)
    fun getIndexFromPath(path: String): Int = nunchukAndroid.getIndexFromPath(path)

    @Throws(NCNativeException::class)
    fun getAddressPath(walletId: String, address: String): String =
        nunchukAndroid.getAddressPath(walletId = walletId, address = address)

    @Throws(NCNativeException::class)
    fun getCoinsFromTxInputs(
        walletId: String,
        inputs: List<TxInput>,
    ): List<UnspentOutput> = nunchukAndroid.getCoinsFromTxInputs(walletId, inputs)

    @Throws(NCNativeException::class)
    fun replaceTransactionId(
        walletId: String,
        txId: String,
        newTxId: String,
    ) = nunchukAndroid.replaceTransactionId(walletId, txId, newTxId)

    @Throws(NCNativeException::class)
    fun createHotWallet() = nunchukAndroid.createHotWallet()

    @Throws(NCNativeException::class)
    fun getMnemonicFromHotWallet(walletId: String) =
        nunchukAndroid.getMnemonicFromHotWallet(walletId)

    @Throws(NCNativeException::class)
    fun markHotWalletExported(walletId: String) = nunchukAndroid.markHotWalletExported(walletId)

    @Throws(NCNativeException::class)
    fun recoverHotWallet(
        mnemonic: String,
        replace: Boolean
    ) = nunchukAndroid.recoverHotWallet(mnemonic, replace)

    @Throws(NCNativeException::class)
    fun exportBBQRWallet(wallet: Wallet, density: Int) =
        nunchukAndroid.exportBBQRWallet(wallet.toBridge(), density)

    @Throws(NCNativeException::class)
    fun exportBBQRTransaction(psbt: String, density: Int) =
        nunchukAndroid.exportBBQRTransaction(psbt, density)

    @Throws(NCNativeException::class)
    fun createSoftwareSignerFromMasterXprv(
        name: String,
        xprv: String,
        isPrimary: Boolean,
        replace: Boolean,
    ) = nunchukAndroid.createSoftwareSignerFromMasterXprv(name, xprv, isPrimary, replace)

    @Throws(NCNativeException::class)
    fun isValidXPrv(xprv: String) = nunchukAndroid.isValidXPrv(xprv)

    @Throws(NCNativeException::class)
    fun getBip32Path(
        walletType: WalletType,
        addressType: AddressType,
        index: Int
    ) = nunchukAndroid.getBip32Path(walletType.ordinal, addressType.ordinal, index)

    @Throws(NCNativeException::class)
    fun parseSignerString(xpub: String) = nunchukAndroid.parseSignerString(xpub)

    @Throws(NCNativeException::class)
    fun estimateRollOverTransactionCount(
        walletId: String,
        tags: List<CoinTag>,
        collections: List<CoinCollection>
    ) = nunchukAndroid.estimateRollOverTransactionCount(walletId, tags, collections)

    @Throws(NCNativeException::class)
    fun estimateRollOverAmount(
        walletId: String,
        newWalletId: String,
        tags: List<CoinTag>,
        collections: List<CoinCollection>,
        feeRate: Amount
    ) = nunchukAndroid.estimateRollOverAmount(walletId, newWalletId, tags, collections, feeRate)

    @Throws(NCNativeException::class)
    fun draftRollOverTransactions(
        walletId: String,
        newWalletId: String,
        tags: List<CoinTag>,
        collections: List<CoinCollection>,
        feeRate: Amount
    ) = nunchukAndroid.draftRollOverTransactions(walletId, newWalletId, tags, collections, feeRate)

    @Throws(NCNativeException::class)
    fun createRollOverTransactions(
        walletId: String,
        newWalletId: String,
        tags: List<CoinTag>,
        collections: List<CoinCollection>,
        feeRate: Amount
    ) = nunchukAndroid.createRollOverTransactions(walletId, newWalletId, tags, collections, feeRate)

    @Throws(NCNativeException::class)
    fun exportWalletToPortal(
        walletId: String,
    ) = nunchukAndroid.exportWalletToPortal(walletId)

    @Throws(NCNativeException::class)
    fun getAddressIndex(
        walletId: String,
        address: String,
    ) = nunchukAndroid.getAddressIndex(walletId, address)

    @Throws(NCNativeException::class)
    fun createNewDecoyPin(
        storagePath: String,
        pin: String,
    ) = nunchukAndroid.createNewDecoyPin(storagePath, pin)

    @Throws(NCNativeException::class)
    fun changeDecoyPin(
        storagePath: String,
        oldPin: String,
        newPin: String,
    ) = nunchukAndroid.changeDecoyPin(storagePath, oldPin, newPin)

    @Throws(NCNativeException::class)
    fun decoyPinExists(
        storagePath: String,
        pin: String
    ) = nunchukAndroid.decoyPinExists(storagePath, pin)

    @Throws(NCNativeException::class)
    fun markAddressAsUsed(
        walletId: String,
        address: String,
    ) = nunchukAndroid.markAddressAsUsed(walletId, address)

    @Throws(NCNativeException::class)
    fun cloneWallet(
        walletId: String,
        decoyPin: String,
    ) = nunchukAndroid.cloneWallet(walletId, decoyPin)

    @Throws(NCNativeException::class)
    fun getFreeGroupWalletConfig(addressType: AddressType): FreeGroupConfig =
        nunchukAndroid.getFreeGroupWalletConfig(addressType.ordinal)

    @Throws(NCNativeException::class)
    fun updateGroupSandbox(
        groupId: String,
        name: String,
        m: Int,
        n: Int,
        addressType: Int,
    ) = nunchukAndroid.updateGroupSandbox(groupId, name, m, n, addressType)

    @Throws(NCNativeException::class)
    fun recoverFreeGroupWallet(filePath: String, name: String, description: String): Wallet =
        nunchukAndroid.recoverFreeGroupWallet(filePath, name, description)

    @Throws(NCNativeException::class)
    fun addSignerToGroup(
        groupId: String,
        signer: SingleSigner,
        index: Int
    ) = nunchukAndroid.addSignerToGroup(groupId, signer, index)

    @Throws(NCNativeException::class)
    fun removeSignerFromGroup(
        groupId: String,
        index: Int
    ) = nunchukAndroid.removeSignerFromGroup(groupId, index)

    @Throws(NCNativeException::class)
    fun finalizeGroup(
        groupId: String,
    ) = nunchukAndroid.finalizeGroup(groupId)

    @Throws(NCNativeException::class)
    fun getGroupWallets() = nunchukAndroid.getGroupWallets()

    @Throws(NCNativeException::class)
    fun getGroups() = nunchukAndroid.getGroups()

    @Throws(NCNativeException::class)
    fun enableGroupWallet(
        osName: String, osVersion: String, appVersion: String, deviceClass: String, deviceId: String, accessToken: String
    ) = nunchukAndroid.enableGroupWallet(osName, osVersion, appVersion, deviceClass, deviceId, accessToken)

    @Throws(NCNativeException::class)
    fun joinGroupWallet(
        sandboxUrl: String,
    ): GroupSandbox = nunchukAndroid.joinGroupWallet(sandboxUrl)

    @Throws(NCNativeException::class)
    fun sendGroupWalletMessage(
        walletId: String,
        message: String,
        singleSigner: SingleSigner
    ) = nunchukAndroid.sendGroupWalletMessage(walletId, message, singleSigner)

    @Throws(NCNativeException::class)
    fun getGroupWalletMessages(
        walletId: String,
        page: Int,
        pageSize: Int
    ) = nunchukAndroid.getGroupWalletMessages(walletId, page, pageSize)

    @Throws(NCNativeException::class)
    fun updateGroupWalletConfig(
        walletId: String,
        chatRetentionDays: Int,
    ) = nunchukAndroid.updateGroupWalletConfig(walletId, chatRetentionDays)

    @Throws(NCNativeException::class)
    fun getGroupWalletConfig(
        walletId: String,
    ) = nunchukAndroid.getGroupWalletConfig(walletId)

    @Throws(NCNativeException::class)
    fun getGroupSandbox(
        groupId: String,
    ) = nunchukAndroid.getGroupSandbox(groupId)

    @Throws(NCNativeException::class)
    fun createGroupSandbox(
        name: String,
        m: Int,
        n: Int,
        addressType: AddressType,
    ) = nunchukAndroid.createGroupSandbox(name, m, n, addressType.ordinal)

    @Throws(NCNativeException::class)
    fun deleteGroupSandbox(
        groupId: String,
    ) = nunchukAndroid.deleteGroupSandbox(groupId)
}