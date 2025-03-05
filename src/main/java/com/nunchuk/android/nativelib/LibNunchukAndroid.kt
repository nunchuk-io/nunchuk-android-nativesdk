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
import com.nunchuk.android.model.bridge.WalletBridge
import com.nunchuk.android.type.HealthStatus
import com.nunchuk.android.type.SignerTag
import com.nunchuk.android.type.SignerType

internal const val LIB_NAME = "nunchuk-android"

internal class LibNunchukAndroid {

    @Throws(NCNativeException::class)
    external fun initNunchuk(
        chain: Int,
        hwiPath: String,
        enableProxy: Boolean,
        testnetServers: List<String>,
        mainnetServers: List<String>,
        signetServers: List<String>,
        backendType: Int,
        storagePath: String,
        passphrase: String,
        accountId: String,
        deviceId: String,
        decoyPin: String,
        baseApiUrl: String,
    )

    @Throws(NCNativeException::class)
    external fun registerGlobalListener()

    @Throws(NCNativeException::class)
    external fun startConsumeGroupEvent()

    @Throws(NCNativeException::class)
    external fun stopConsumeGroupEvent()

    @Throws(NCNativeException::class)
    external fun createSigner(
        name: String,
        xpub: String,
        publicKey: String,
        derivationPath: String,
        masterFingerprint: String,
        type: SignerType,
        tags: List<SignerTag>,
        replace: Boolean,
    ): SingleSigner

    @Throws(NCNativeException::class)
    external fun parseKeystoneSigner(
        qrData: String,
    ): SingleSigner

    @Throws(NCNativeException::class)
    external fun createSoftwareSigner(
        name: String,
        mnemonic: String,
        passphrase: String = "",
        isPrimary: Boolean,
        replace: Boolean,
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
        description: String,
        decoyPin: String,
    ): Wallet

    @Throws(NCNativeException::class)
    external fun getWallets(): List<Wallet>

    @Throws(NCNativeException::class)
    external fun getMostRecentlyUsedWallets(): List<Wallet>

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
        description: String,
    ): String

    @Throws(NCNativeException::class)
    external fun exportWallet(
        walletId: String,
        filePath: String,
        format: Int,
    ): Boolean

    @Throws(NCNativeException::class)
    external fun importKeystoneWallet(
        qrData: List<String>,
        description: String,
    ): Wallet

    @Throws(NCNativeException::class)
    external fun getWallet(walletId: String): Wallet

    @Throws(NCNativeException::class)
    external fun updateWallet(wallet: WalletBridge): Boolean

    @Throws(NCNativeException::class)
    external fun exportKeystoneWallet(walletId: String, density: Int): List<String>

    @Throws(NCNativeException::class)
    external fun exportBCR2020010Wallet(walletId: String, density: Int): List<String>

    @Throws(NCNativeException::class)
    external fun generateMnemonic(count: Int): String

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
        addressType: Int,
    ): SingleSigner

    @Throws(NCNativeException::class)
    external fun getDefaultSignerFromMasterSigner(
        masterSignerId: String,
        walletType: Int,
        addressType: Int,
    ): SingleSigner

    @Throws(NCNativeException::class)
    external fun broadcastTransaction(walletId: String, txId: String): Transaction

    @Throws(NCNativeException::class)
    external fun createTransaction(
        walletId: String,
        outputs: Map<String, Amount>,
        memo: String,
        inputs: List<TxInput>,
        feeRate: Amount,
        subtractFeeFromAmount: Boolean,
        replaceTxId: String,
    ): Transaction

    @Throws(NCNativeException::class)
    external fun createInheritanceClaimTransaction(
        signers: List<SingleSigner>,
        psbt: String,
        subAmount: String,
        feeRate: String,
        fee: String,
        isDraft: Boolean,
    ): Transaction

    @Throws(NCNativeException::class)
    external fun draftTransaction(
        walletId: String,
        outputs: Map<String, Amount>,
        inputs: List<TxInput>,
        feeRate: Amount,
        subtractFeeFromAmount: Boolean,
        replaceTxId: String,
    ): Transaction

    @Throws(NCNativeException::class)
    external fun deleteTransaction(walletId: String, txId: String): Boolean

    @Throws(NCNativeException::class)
    external fun exportKeystoneTransaction(
        walletId: String,
        txId: String,
        density: Int,
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
    external fun importKeystoneTransaction(walletId: String, qrData: List<String>): Transaction

    @Throws(NCNativeException::class)
    external fun updateTransactionMemo(walletId: String, txId: String, newMemo: String): Boolean

    @Throws(NCNativeException::class)
    external fun updateTransaction(
        walletId: String,
        txId: String,
        newTxId: String,
        rawTx: String,
        rejectMsg: String,
    ): Transaction

    @Throws(NCNativeException::class)
    external fun signTransaction(walletId: String, txId: String, device: Device): Transaction

    @Throws(NCNativeException::class)
    external fun signAirgapTransaction(
        initEventId: String,
        masterSignerId: String,
    ): NunchukMatrixEvent

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
    external fun clearSignerPassphrase(masterSignerId: String)

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
        isEscrow: Boolean,
        des: String,
        signers: List<SingleSigner>,
    ): NunchukMatrixEvent

    @Throws(NCNativeException::class)
    external fun joinSharedWallet(roomId: String, signer: SingleSigner): NunchukMatrixEvent

    @Throws(NCNativeException::class)
    external fun leaveSharedWallet(
        roomId: String,
        joinEventId: String,
        reason: String,
    ): NunchukMatrixEvent

    @Throws(NCNativeException::class)
    external fun cancelSharedWallet(roomId: String, reason: String): NunchukMatrixEvent

    @Throws(NCNativeException::class)
    external fun createSharedWallet(roomId: String): NunchukMatrixEvent

    @Throws(NCNativeException::class)
    external fun consumeEvent(event: NunchukMatrixEvent)

    @Throws(NCNativeException::class)
    external fun hasRoomWallet(roomId: String): Boolean

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
        subtractFeeFromAmount: Boolean,
    ): NunchukMatrixEvent

    @Throws(NCNativeException::class)
    external fun signRoomTransaction(
        initEventId: String,
        device: Device,
    ): NunchukMatrixEvent

    @Throws(NCNativeException::class)
    external fun rejectRoomTransaction(
        initEventId: String,
        reason: String,
    ): NunchukMatrixEvent

    @Throws(NCNativeException::class)
    external fun cancelRoomTransaction(
        initEventId: String,
        reason: String,
    ): NunchukMatrixEvent

    @Throws(NCNativeException::class)
    external fun broadcastRoomTransaction(
        initEventId: String,
    ): NunchukMatrixEvent

    @Throws(NCNativeException::class)
    external fun getRoomTransaction(
        initEventId: String,
    ): RoomTransaction

    @Throws(NCNativeException::class)
    external fun getPendingTransactions(
        roomId: String,
    ): List<RoomTransaction>

    @Throws(NCNativeException::class)
    external fun getTransactionId(
        eventId: String,
    ): String

    @Throws(NCNativeException::class)
    external fun consumeSyncEvent(
        event: NunchukMatrixEvent,
    )

    @Throws(NCNativeException::class)
    external fun enableAutoBackUp(
        enable: Boolean,
    )

    @Throws(NCNativeException::class)
    external fun registerAutoBackUp(
        syncRoomId: String,
        accessToken: String,
    )

    @Throws(NCNativeException::class)
    external fun uploadFileCallback(
        fileJsonInfo: String,
        fileUrl: String,
    )

    @Throws(NCNativeException::class)
    external fun registerDownloadFileBackup()

    @Throws(NCNativeException::class)
    external fun downloadFileCallback(
        fileJsonInfo: String,
        fileData: ByteArray,
    )

    @Throws(NCNativeException::class)
    external fun writeFileCallback(
        fileJsonInfo: String,
        fileData: String,
    )

    @Throws(NCNativeException::class)
    external fun importWallet(filePath: String, name: String, description: String): Wallet

    @Throws(NCNativeException::class)
    external fun parsePassportSigners(qrData: List<String>): List<SingleSigner>

    @Throws(NCNativeException::class)
    external fun healthCheckMasterSigner(
        fingerprint: String,
        message: String,
        signature: String,
        path: String,
    ): HealthStatus

    @Throws(NCNativeException::class)
    external fun sendErrorEvent(
        roomId: String,
        platform: String,
        code: String,
        message: String,
    )

    @Throws(NCNativeException::class)
    external fun getMatrixEvent(
        eventId: String,
    ): NunchukMatrixEvent

    @Throws(NCNativeException::class)
    external fun backup()

    @Throws(NCNativeException::class)
    external fun tapSignerStatus(
        isoDep: IsoDep,
    ): TapSignerStatus

    @Throws(NCNativeException::class)
    external fun initTapSigner(
        isoDep: IsoDep,
        oldCvc: String,
        chainCode: String,
    )

    @Throws(NCNativeException::class)
    external fun createTapSigner(
        isoDep: IsoDep,
        cvc: String,
        name: String,
        replace: Boolean
    ): MasterSigner

    @Throws(NCNativeException::class)
    external fun signTransactionByTapSigner(
        isoDep: IsoDep,
        cvc: String,
        walletId: String,
        txId: String,
    ): Transaction

    @Throws(NCNativeException::class)
    external fun signRoomTransactionByTapSigner(
        isoDep: IsoDep,
        cvc: String,
        initEventId: String,
    ): NunchukMatrixEvent

    @Throws(NCNativeException::class)
    external fun getBackupTapSignerKey(
        isoDep: IsoDep,
        cvc: String,
        masterSignerId: String,
    ): TapSignerStatus

    @Throws(NCNativeException::class)
    external fun changeCvcTapSigner(
        isoDep: IsoDep,
        oldCvc: String,
        newCvc: String,
        masterSignerId: String,
    ): Boolean

    @Throws(NCNativeException::class)
    external fun healthCheckTapSigner(
        isoDep: IsoDep,
        cvc: String,
        fingerprint: String,
        message: String,
        signature: String,
        path: String,
    ): HealthStatus

    @Throws(NCNativeException::class)
    external fun waitTapSigner(
        isoDep: IsoDep,
    ): TapSignerStatus

    @Throws(NCNativeException::class)
    external fun generateRandomChainCode(): String

    @Throws(NCNativeException::class)
    external fun tapSignerTopUpXpub(
        isoDep: IsoDep,
        cvc: String,
        masterSignerId: String,
    )

    @Throws(NCNativeException::class)
    external fun cacheDefaultTapsignerMasterSignerXPub(
        isoDep: IsoDep,
        cvc: String,
        masterSignerId: String,
    )

    @Throws(NCNativeException::class)
    external fun getTapSignerStatusFromMasterSigner(
        masterSignerId: String,
    ): TapSignerStatus

    @Throws(NCNativeException::class)
    external fun decryptBackUpKey(
        backUpKey: String,
        decryptionKey: String,
        rawName: String,
    ): MasterSigner

    @Throws(NCNativeException::class)
    external fun importTapsignerMasterSignerContent(
        content: ByteArray,
        backUpKey: String,
        rawName: String,
    ): MasterSigner

    @Throws(NCNativeException::class)
    external fun importBackupKeyContent(
        content: ByteArray,
        backUpKey: String,
        rawName: String,
    ): MasterSigner

    @Throws(NCNativeException::class)
    external fun parseWalletDescriptor(content: String): Wallet

    @Throws(NCNativeException::class)
    external fun parseKeystoneWallet(chain: Int, qrData: List<String>): Wallet?

    @Throws(NCNativeException::class)
    external fun hasSigner(signer: SingleSigner): Boolean

    @Throws(NCNativeException::class)
    external fun getSigner(signer: SingleSigner): SingleSigner

    @Throws(NCNativeException::class)
    external fun getSatsCardStatus(
        isoDep: IsoDep,
    ): SatsCardStatus

    @Throws(NCNativeException::class)
    external fun setupSatsCard(
        isoDep: IsoDep,
        cvc: String,
        chainCode: String,
    ): SatsCardStatus

    @Throws(NCNativeException::class)
    external fun loadSatsCardSlotsBalance(
        slots: List<SatsCardSlot>,
    ): List<SatsCardSlot>

    @Throws(NCNativeException::class)
    external fun unsealSatsCard(
        isoDep: IsoDep,
        cvc: String,
        slot: SatsCardSlot,
    ): SatsCardSlot

    @Throws(NCNativeException::class)
    external fun sweepBalance(
        slots: List<SatsCardSlot>,
        address: String,
        feeRate: Int,
    ): Transaction

    @Throws(NCNativeException::class)
    external fun draftSatscardTransaction(
        slots: List<SatsCardSlot>,
        address: String,
        feeRate: Int,
    ): Transaction

    @Throws(NCNativeException::class)
    external fun getSlotKeys(
        isoDep: IsoDep,
        cvc: String,
        slots: List<SatsCardSlot>,
    ): List<SatsCardSlot>

    @Throws(NCNativeException::class)
    external fun getAutoCardStatus(
        isoDep: IsoDep,
    ): CardStatus

    @Throws(NCNativeException::class)
    external fun waitAutoCard(
        isoDep: IsoDep,
    )

    @Throws(NCNativeException::class)
    external fun loadTransaction(
        txId: String,
    ): Transaction

    @Throws(NCNativeException::class)
    external fun getMk4Signers(
        records: Array<NdefRecord>,
    ): List<SingleSigner>

    @Throws(NCNativeException::class)
    external fun createMk4Signer(
        signer: SingleSigner,
    ): SingleSigner

    @Throws(NCNativeException::class)
    external fun exportWalletToMk4(
        walletId: String,
    ): Array<NdefRecord>

    @Throws(NCNativeException::class)
    external fun exportWalletToBsmsById(
        walletId: String,
    ): String

    @Throws(NCNativeException::class)
    external fun exportWalletToBsms(
        wallet: WalletBridge,
    ): String

    @Throws(NCNativeException::class)
    external fun exportPsbtToMk4(
        walletId: String,
        txId: String,
    ): Array<NdefRecord>

    @Throws(NCNativeException::class)
    external fun exportRawPsbtToMk4(
        psbt: String,
    ): Array<NdefRecord>

    @Throws(NCNativeException::class)
    external fun importTransactionFromMk4(
        walletId: String,
        records: Array<NdefRecord>,
    ): Transaction?

    @Throws(NCNativeException::class)
    external fun importWalletFromMk4(
        chain: Int,
        records: Array<NdefRecord>,
    ): Wallet?

    @Throws(NCNativeException::class)
    external fun parseWalletFromMk4(
        chain: Int,
        records: Array<NdefRecord>,
    ): Wallet?

    @Throws(NCNativeException::class)
    external fun getColdCardWallets(records: Array<NdefRecord>): List<Wallet>?

    @Throws(NCNativeException::class)
    external fun createWallet2(wallet: WalletBridge): Wallet

    @Throws(NCNativeException::class)
    external fun getPrimaryKeyAddress(
        mnemonic: String,
        passphrase: String,
    ): String?

    @Throws(NCNativeException::class)
    external fun signLoginMessage(
        mnemonic: String,
        passphrase: String,
        message: String,
    ): String?

    @Throws(NCNativeException::class)
    external fun signLoginMessageImpl(
        masterSignerId: String,
        message: String,
    ): String?

    @Throws(NCNativeException::class)
    external fun getMasterFingerprint(
        mnemonic: String,
        passphrase: String,
    ): String?

    @Throws(NCNativeException::class)
    external fun getPrimaryKeys(chain: Int, storagePath: String): List<PrimaryKey>

    @Throws(NCNativeException::class)
    external fun parseBtcUri(
        uri: String,
    ): BtcUri

    @Throws(NCNativeException::class)
    external fun setSelectedWallet(walletId: String): Boolean

    @Throws(NCNativeException::class)
    external fun deletePrimaryKey(): Boolean

    @Throws(NCNativeException::class)
    external fun generateColdCardHealthCheckMessage(
        derivationPath: String,
        message: String?,
    ): Array<NdefRecord>

    @Throws(NCNativeException::class)
    external fun healthCheckColdCard(
        signer: SingleSigner,
        records: Array<NdefRecord>,
    ): ColdCardHealth

    @Throws(NCNativeException::class)
    external fun isValidDerivationPath(path: String): Boolean

    @Throws(NCNativeException::class)
    external fun getSignerFromMasterSigner(masterSignerId: String, path: String): SingleSigner

    @Throws(NCNativeException::class)
    external fun getSignerFromMasterSignerByIndex(
        masterSignerId: String,
        walletType: Int,
        addressType: Int,
        index: Int,
    ): SingleSigner

    @Throws(NCNativeException::class)
    external fun getSignerFromTapsignerMasterSigner(
        isoDep: IsoDep,
        cvc: String,
        masterSignerId: String,
        walletType: Int,
        addressType: Int,
        index: Int,
    ): SingleSigner?

    @Throws(NCNativeException::class)
    external fun parseJsonSigners(
        str: String,
        type: SignerType,
    ): List<SingleSigner>

    @Throws(NCNativeException::class)
    external fun verifyTapSignerBackup(
        backUpKey: String,
        decryptionKey: String,
        masterSignerId: String,
    ): Boolean

    @Throws(NCNativeException::class)
    external fun verifyTapSignerBackupContent(
        content: ByteArray,
        backUpKey: String,
        masterSignerId: String,
    ): Boolean

    @Throws(NCNativeException::class)
    external fun verifyColdCardBackup(
        data: ByteArray,
        backUpKey: String,
        masterSignerId: String,
    )

    @Throws(NCNativeException::class)
    external fun hasWallet(
        walletId: String,
    ): Boolean

    @Throws(NCNativeException::class)
    external fun addTapSigner(
        cardId: String,
        xfp: String,
        name: String,
        version: String,
        brithHeight: Int,
        isTestNet: Boolean,
        replace: Boolean
    )

    @Throws(NCNativeException::class)
    external fun signerTypeFromStr(signerType: String): SignerType

    @Throws(NCNativeException::class)
    external fun importPsbt(walletId: String, psbt: String): Transaction

    @Throws(NCNativeException::class)
    external fun signHealthCheckMessage(signer: SingleSigner, messagesToSign: String): String

    @Throws(NCNativeException::class)
    external fun signHealthCheckMessageTapSigner(
        isoDep: IsoDep,
        cvc: String,
        signer: SingleSigner,
        messagesToSign: String,
    ): String

    @Throws(NCNativeException::class)
    external fun signHealthCheckMessageTapSignerSignIn(
        signer: SingleSigner,
        isoDep: IsoDep,
        cvc: String,
        messagesToSign: String,
    ): String

    @Throws(NCNativeException::class)
    external fun signMessageColdCard(derivationPath: String, messagesToSign: String): String

    @Throws(NCNativeException::class)
    external fun createRequestToken(signature: String, fingerprint: String): String

    @Throws(NCNativeException::class)
    external fun getHealthCheckMessage(body: String): String

    @Throws(NCNativeException::class)
    external fun getHealthCheckDummyTxMessage(walletId: String, body: String): String

    @Throws(NCNativeException::class)
    external fun getSignInHealthCheckDummyTxMessage(body: String): String

    @Throws(NCNativeException::class)
    external fun getDummyTx(walletId: String, message: String): Transaction

    @Throws(NCNativeException::class)
    external fun getSignInDummyTx(psbt: String): Transaction

    @Throws(NCNativeException::class)
    external fun getDummyTxByByteArray(walletId: String, fileData: ByteArray): Transaction?

    @Throws(NCNativeException::class)
    external fun getSignInDummyTxByByteArray(fileData: ByteArray): Transaction?

    @Throws(NCNativeException::class)
    external fun exportKeystoneDummyTransaction(txToSign: String, density: Int): List<String>

    @Throws(NCNativeException::class)
    external fun parseKeystoneDummyTransaction(qrs: List<String>): String

    @Throws(NCNativeException::class)
    external fun getDummyTransactionSignature(signer: SingleSigner, psbt: String): String

    external fun getColdcardSignatureFromPsbt(
        signer: SingleSigner,
        records: Array<NdefRecord>,
    ): String?

    external fun updateTransactionSchedule(walletId: String, txId: String, broadcastTime: Long)

    @Throws(NCNativeException::class)
    external fun forceRefreshWallet(walletId: String)

    @Throws(NCNativeException::class)
    external fun analyzeQr(qrs: List<String>): Double

    @Throws(NCNativeException::class)
    external fun hashSHA256(data: String): String

    @Throws(NCNativeException::class)
    external fun lockCoin(walletId: String, txId: String, vout: Int): Boolean

    @Throws(NCNativeException::class)
    external fun unlockCoin(walletId: String, txId: String, vout: Int): Boolean

    @Throws(NCNativeException::class)
    external fun addToCoinTag(walletId: String, txId: String, tagId: Int, vout: Int): Boolean

    @Throws(NCNativeException::class)
    external fun removeFromCoinTag(walletId: String, txId: String, tagId: Int, vout: Int): Boolean

    @Throws(NCNativeException::class)
    external fun addToCoinCollection(
        walletId: String,
        txId: String,
        collectionId: Int,
        vout: Int,
    ): Boolean

    @Throws(NCNativeException::class)
    external fun removeFromCoinCollection(
        walletId: String,
        txId: String,
        collectionId: Int,
        vout: Int,
    ): Boolean

    @Throws(NCNativeException::class)
    external fun getCoinByTag(walletId: String, tagId: Int): List<UnspentOutput>

    @Throws(NCNativeException::class)
    external fun getCoinInCollection(walletId: String, collectionId: Int): List<UnspentOutput>

    @Throws(NCNativeException::class)
    external fun getCoinTags(walletId: String): List<CoinTag>

    @Throws(NCNativeException::class)
    external fun createCoinTag(walletId: String, name: String, color: String): CoinTag

    @Throws(NCNativeException::class)
    external fun updateCoinTag(walletId: String, coinTag: CoinTag): Boolean

    @Throws(NCNativeException::class)
    external fun deleteCoinTag(walletId: String, tagId: Int): Boolean

    @Throws(NCNativeException::class)
    external fun getCoinCollections(walletId: String): List<CoinCollection>

    @Throws(NCNativeException::class)
    external fun createCoinCollection(walletId: String, name: String): CoinCollection

    @Throws(NCNativeException::class)
    external fun updateCoinCollection(
        walletId: String,
        coinCollection: CoinCollection,
        applyToExistingCoin: Boolean
    ): Boolean

    @Throws(NCNativeException::class)
    external fun deleteCoinCollection(walletId: String, collectionId: Int): Boolean

    @Throws(NCNativeException::class)
    external fun isMyAddress(walletId: String, address: String): Boolean

    @Throws(NCNativeException::class)
    external fun exportCoinControlData(walletId: String): String

    @Throws(NCNativeException::class)
    external fun importCoinControlData(walletId: String, data: String, force: Boolean): Boolean

    @Throws(NCNativeException::class)
    external fun exportCoinControlBIP329(walletId: String): String

    @Throws(NCNativeException::class)
    external fun importCoinControlBIP329(walletId: String, data: String)

    @Throws(NCNativeException::class)
    external fun getCoinAncestry(
        walletId: String,
        txId: String,
        vout: Int,
    ): List<List<UnspentOutput>>

    @Throws(NCNativeException::class)
    external fun signMessageByTapSigner(
        isoDep: IsoDep,
        cvc: String,
        message: String,
        path: String,
        masterSignerId: String,
    ): SignedMessage?

    @Throws(NCNativeException::class)
    external fun signMessageBySoftwareKey(
        message: String,
        path: String,
        masterSignerId: String,
    ): SignedMessage?

    @Throws(NCNativeException::class)
    external fun getHealthCheckPath(): String?

    @Throws(NCNativeException::class)
    external fun getRawTransaction(walletId: String, txId: String): String?

    @Throws(NCNativeException::class)
    external fun importDummyTx(dummyTransactionJson: String): Transaction?

    @Throws(NCNativeException::class)
    external fun deleteDummyTx(walletId: String, txId: String): Boolean

    @Throws(NCNativeException::class)
    external fun saveDummyTxRequestToken(
        walletId: String,
        txId: String,
        requestToken: String,
    ): Map<String, Boolean>

    @Throws(NCNativeException::class)
    external fun getDummyTxRequestToken(
        walletId: String,
        txId: String,
    ): Map<String, Boolean>

    @Throws(NCNativeException::class)
    external fun getDummyTransactions(
        walletId: String,
    ): List<Transaction>

    @Throws(NCNativeException::class)
    external fun getDummyTransaction(
        walletId: String,
        txId: String,
    ): Transaction?

    @Throws
    external fun getCurrentSignerIndex(
        xfp: String,
        walletType: Int,
        addressType: Int,
    ): Int

    @Throws
    external fun getSignerByIndex(
        xfp: String,
        walletType: Int,
        addressType: Int,
        index: Int,
    ): SingleSigner?

    external fun getAddressByIndex(
        wallet: WalletBridge,
        from: Int,
        to: Int,
    ): List<String>

    @Throws(NCNativeException::class)
    external fun getIndexFromPath(path: String): Int

    @Throws(NCNativeException::class)
    external fun getAddressPath(walletId: String, address: String): String

    @Throws(NCNativeException::class)
    external fun getCoinsFromTxInputs(
        walletId: String,
        inputs: List<TxInput>,
    ): List<UnspentOutput>

    @Throws(NCNativeException::class)
    external fun replaceTransactionId(
        walletId: String,
        txId: String,
        replaceTxId: String,
    )

    @Throws(NCNativeException::class)
    external fun createHotWallet(): Wallet?

    @Throws(NCNativeException::class)
    external fun getMnemonicFromHotWallet(walletId: String): String

    @Throws(NCNativeException::class)
    external fun markHotWalletExported(walletId: String)

    @Throws(NCNativeException::class)
    external fun recoverHotWallet(
        mnemonic: String,
        replace: Boolean,
    ): Wallet?

    @Throws(NCNativeException::class)
    external fun exportBBQRWallet(wallet: WalletBridge, density: Int): List<String>

    @Throws(NCNativeException::class)
    external fun exportBBQRTransaction(psbt: String, density: Int): List<String>

    @Throws(NCNativeException::class)
    external fun createSoftwareSignerFromMasterXprv(
        name: String,
        xprv: String,
        isPrimary: Boolean,
        replace: Boolean,
    ): MasterSigner?

    @Throws(NCNativeException::class)
    external fun isValidXPrv(xprv: String): Boolean

    @Throws(NCNativeException::class)
    external fun getBip32Path(
        walletType: Int,
        addressType: Int,
        index: Int
    ): String

    @Throws(NCNativeException::class)
    external fun parseSignerString(
        signerString: String,
    ): SingleSigner?

    @Throws(NCNativeException::class)
    external fun estimateRollOverTransactionCount(
        walletId: String,
        tags: List<CoinTag>,
        collections: List<CoinCollection>
    ): Int

    @Throws(NCNativeException::class)
    external fun estimateRollOverAmount(
        walletId: String,
        newWalletId: String,
        tags: List<CoinTag>,
        collections: List<CoinCollection>,
        feeRate: Amount
    ): PairAmount

    @Throws(NCNativeException::class)
    external fun draftRollOverTransactions(
        walletId: String,
        newWalletId: String,
        tags: List<CoinTag>,
        collections: List<CoinCollection>,
        feeRate: Amount
    ): Array<DraftRollOverTransaction>

    @Throws(NCNativeException::class)
    external fun createRollOverTransactions(
        walletId: String,
        newWalletId: String,
        tags: List<CoinTag>,
        collections: List<CoinCollection>,
        feeRate: Amount
    ): List<Transaction>

    @Throws(NCNativeException::class)
    external fun exportWalletToPortal(
        walletId: String,
    ): BSMSData?

    @Throws(NCNativeException::class)
    external fun getAddressIndex(
        walletId: String,
        address: String,
    ): Int

    @Throws(NCNativeException::class)
    external fun createNewDecoyPin(
        storagePath: String,
        pin: String,
    )

    @Throws(NCNativeException::class)
    external fun changeDecoyPin(
        storagePath: String,
        oldPin: String,
        newPin: String,
    ): Boolean

    @Throws(NCNativeException::class)
    external fun decoyPinExists(
        storagePath: String,
        pin: String
    ): Boolean

    @Throws(NCNativeException::class)
    external fun markAddressAsUsed(
        walletId: String,
        address: String
    ): Boolean

    @Throws(NCNativeException::class)
    external fun cloneWallet(
        walletId: String,
        decoyPin: String,
    ): Wallet

    @Throws(NCNativeException::class)
    external fun getGlobalGroupWalletConfig(addressType: Int): GlobalGroupWalletConfig

    @Throws(NCNativeException::class)
    external fun updateGroupSandbox(
        groupId: String,
        name: String,
        m: Int,
        n: Int,
        addressType: Int
    ): GroupSandbox

    @Throws(NCNativeException::class)
    external fun recoverFreeGroupWallet(filePath: String, name: String, description: String)

    @Throws(NCNativeException::class)
    external fun checkGroupWalletExists(walletId: String): Boolean

    @Throws(NCNativeException::class)
    external fun addSignerToGroup(
        groupId: String,
        signer: SingleSigner,
        index: Int
    ): GroupSandbox

    @Throws(NCNativeException::class)
    external fun removeSignerFromGroup(
        groupId: String,
        index: Int
    ): GroupSandbox

    @Throws(NCNativeException::class)
    external fun finalizeGroup(
        groupId: String,
        keySetIndexes: Set<Int>
    ): GroupSandbox

    @Throws(NCNativeException::class)
    external fun getGroupWallets(): List<Wallet>

    @Throws(NCNativeException::class)
    external fun getGroups(): List<GroupSandbox>

    @Throws(NCNativeException::class)
    external fun enableGroupWallet(
        osName: String,
        osVersion: String,
        appVersion: String,
        deviceClass: String,
        deviceId: String,
        accessToken: String
    )

    @Throws(NCNativeException::class)
    external fun joinGroupWallet(
        sandboxUrl: String,
    ): GroupSandbox

    @Throws(NCNativeException::class)
    external fun sendGroupWalletMessage(
        walletId: String,
        message: String,
        singleSigner: SingleSigner?
    )

    @Throws(NCNativeException::class)
    external fun getGroupWalletMessages(
        walletId: String,
        page: Int,
        pageSize: Int
    ): List<FreeGroupMessage>

    @Throws(NCNativeException::class)
    external fun updateGroupWalletConfig(
        walletId: String,
        chatRetentionDays: Int,
    )

    @Throws(NCNativeException::class)
    external fun getGroupWalletConfig(
        walletId: String,
    ): FreeGroupWalletConfig

    @Throws(NCNativeException::class)
    external fun getGroupSandbox(
        groupId: String,
    ): GroupSandbox?

    @Throws(NCNativeException::class)
    external fun createGroupSandbox(
        name: String,
        m: Int,
        n: Int,
        addressType: Int,
    ): GroupSandbox

    @Throws(NCNativeException::class)
    external fun deleteGroupSandbox(
        groupId: String,
    ): Boolean

    @Throws(NCNativeException::class)
    external fun getGroupOnline(
        groupId: String,
    ): Int

    @Throws(NCNativeException::class)
    external fun getGroupDeviceUID(): String

    @Throws(NCNativeException::class)
    external fun getGroupWalletUnreadMessagesCount(
        walletId: String,
    ): Int

    @Throws(NCNativeException::class)
    external fun setGroupWalletLastReadMessage(
        walletId: String,
        messageId: String,
    )

    @Throws(NCNativeException::class)
    external fun joinGroupWalletById(
        groupId: String,
    ): GroupSandbox

    @Throws(NCNativeException::class)
    external fun setSlotOccupied(
        groupId: String,
        index: Int,
        value: Boolean
    ): GroupSandbox?

    @Throws(NCNativeException::class)
    external fun getSignerFromTapsignerMasterSignerByPath(
        isoDep: IsoDep,
        cvc: String,
        masterSignerId: String,
        path: String
    ): SingleSigner

    @Throws(NCNativeException::class)
    external fun createReplaceGroup(walletId: String): GroupSandbox

    @Throws(NCNativeException::class)
    external fun acceptReplaceGroup(walletId: String, groupId: String): GroupSandbox

    @Throws(NCNativeException::class)
    external fun declineReplaceGroup(walletId: String, groupId: String)

    @Throws(NCNativeException::class)
    external fun getReplaceGroups(walletId: String): Map<String, Boolean>

    @Throws(NCNativeException::class)
    external fun decryptGroupWalletId(walletId: String): String

    @Throws(NCNativeException::class)
    external fun decryptGroupTxId(walletId: String, txId: String): String

    companion object {
        init {
            System.loadLibrary(LIB_NAME)
        }
    }
}
