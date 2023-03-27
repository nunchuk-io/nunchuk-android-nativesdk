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
        deviceId: String
    )

    @Throws(NCNativeException::class)
    external fun createSigner(
        name: String,
        xpub: String,
        publicKey: String,
        derivationPath: String,
        masterFingerprint: String,
        type: SignerType,
        tags: List<SignerTag>
    ): SingleSigner

    @Throws(NCNativeException::class)
    external fun parseKeystoneSigner(
        qrData: String
    ): SingleSigner

    @Throws(NCNativeException::class)
    external fun createSoftwareSigner(
        name: String,
        mnemonic: String,
        passphrase: String = "",
        isPrimary: Boolean
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
        qrData: List<String>,
        description: String
    ): Wallet

    @Throws(NCNativeException::class)
    external fun getWallet(walletId: String): Wallet

    @Throws(NCNativeException::class)
    external fun updateWallet(wallet: WalletBridge): Boolean

    @Throws(NCNativeException::class)
    external fun exportKeystoneWallet(walletId: String, density: Int): List<String>

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
    external fun getDefaultSignerFromMasterSigner(
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
    external fun createTransactionWallet(
        signer: SingleSigner,
        psbt: String,
        subAmount: String,
        feeRate: String,
        fee: String
    ): Transaction

    @Throws(NCNativeException::class)
    external fun draftTransaction(
        walletId: String,
        outputs: Map<String, Amount>,
        inputs: List<TxInput>,
        feeRate: Amount,
        subtractFeeFromAmount: Boolean
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
        rejectMsg: String
    ): Transaction

    @Throws(NCNativeException::class)
    external fun signTransaction(walletId: String, txId: String, device: Device): Transaction

    @Throws(NCNativeException::class)
    external fun signAirgapTransaction(
        initEventId: String,
        masterSignerId: String
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
        signers: List<SingleSigner>
    ): NunchukMatrixEvent

    @Throws(NCNativeException::class)
    external fun joinSharedWallet(roomId: String, signer: SingleSigner): NunchukMatrixEvent

    @Throws(NCNativeException::class)
    external fun leaveSharedWallet(
        roomId: String,
        joinEventId: String,
        reason: String
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

    @Throws(NCNativeException::class)
    external fun consumeSyncEvent(
        event: NunchukMatrixEvent
    )

    @Throws(NCNativeException::class)
    external fun enableAutoBackUp(
        enable: Boolean
    )

    @Throws(NCNativeException::class)
    external fun registerAutoBackUp(
        syncRoomId: String,
        accessToken: String
    )

    @Throws(NCNativeException::class)
    external fun uploadFileCallback(
        fileJsonInfo: String,
        fileUrl: String
    )

    @Throws(NCNativeException::class)
    external fun registerDownloadFileBackup()

    @Throws(NCNativeException::class)
    external fun downloadFileCallback(
        fileJsonInfo: String,
        fileData: ByteArray
    )

    @Throws(NCNativeException::class)
    external fun writeFileCallback(
        fileJsonInfo: String,
        fileData: String
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
        path: String
    ): HealthStatus

    @Throws(NCNativeException::class)
    external fun sendErrorEvent(
        roomId: String,
        platform: String,
        code: String,
        message: String
    )

    @Throws(NCNativeException::class)
    external fun getMatrixEvent(
        eventId: String,
    ): NunchukMatrixEvent

    @Throws(NCNativeException::class)
    external fun backup()

    @Throws(NCNativeException::class)
    external fun tapSignerStatus(
        isoDep: IsoDep
    ): TapSignerStatus

    @Throws(NCNativeException::class)
    external fun initTapSigner(
        isoDep: IsoDep,
        oldCvc: String,
        chainCode: String
    )

    @Throws(NCNativeException::class)
    external fun createTapSigner(
        isoDep: IsoDep,
        cvc: String,
        name: String
    ): MasterSigner

    @Throws(NCNativeException::class)
    external fun signTransactionByTapSigner(
        isoDep: IsoDep,
        cvc: String,
        walletId: String,
        txId: String
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
        masterSignerId: String
    ): TapSignerStatus

    @Throws(NCNativeException::class)
    external fun changeCvcTapSigner(
        isoDep: IsoDep,
        oldCvc: String,
        newCvc: String,
        masterSignerId: String
    ): Boolean

    @Throws(NCNativeException::class)
    external fun healthCheckTapSigner(
        isoDep: IsoDep,
        cvc: String,
        fingerprint: String,
        message: String,
        signature: String,
        path: String
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
        masterSignerId: String
    )

    @Throws(NCNativeException::class)
    external fun cacheDefaultTapsignerMasterSignerXPub(
        isoDep: IsoDep,
        cvc: String,
        masterSignerId: String
    )

    @Throws(NCNativeException::class)
    external fun getTapSignerStatusFromMasterSigner(
        masterSignerId: String
    ): TapSignerStatus

    @Throws(NCNativeException::class)
    external fun decryptBackUpKey(
        backUpKey: String,
        decryptionKey: String,
        rawName: String
    ): MasterSigner

    @Throws(NCNativeException::class)
    external fun importTapsignerMasterSignerContent(
        content: ByteArray,
        backUpKey: String,
        rawName: String
    ): MasterSigner

    @Throws(NCNativeException::class)
    external fun parseWalletDescriptor(content: String): Wallet

    @Throws(NCNativeException::class)
    external fun hasSigner(signer: SingleSigner): Boolean

    @Throws(NCNativeException::class)
    external fun getSatsCardStatus(
        isoDep: IsoDep
    ): SatsCardStatus

    @Throws(NCNativeException::class)
    external fun setupSatsCard(
        isoDep: IsoDep,
        cvc: String,
        chainCode: String
    ): SatsCardStatus

    @Throws(NCNativeException::class)
    external fun loadSatsCardSlotsBalance(
        slots: List<SatsCardSlot>
    ): List<SatsCardSlot>

    @Throws(NCNativeException::class)
    external fun unsealSatsCard(
        isoDep: IsoDep,
        cvc: String,
        slot: SatsCardSlot
    ): SatsCardSlot

    @Throws(NCNativeException::class)
    external fun sweepBalance(
        slots: List<SatsCardSlot>,
        address: String,
        feeRate: Int
    ): Transaction

    @Throws(NCNativeException::class)
    external fun draftSatscardTransaction(
        slots: List<SatsCardSlot>,
        address: String,
        feeRate: Int
    ): Transaction

    @Throws(NCNativeException::class)
    external fun getSlotKeys(
        isoDep: IsoDep,
        cvc: String,
        slots: List<SatsCardSlot>
    ): List<SatsCardSlot>

    @Throws(NCNativeException::class)
    external fun getAutoCardStatus(
        isoDep: IsoDep
    ): CardStatus

    @Throws(NCNativeException::class)
    external fun waitAutoCard(
        isoDep: IsoDep
    )

    @Throws(NCNativeException::class)
    external fun loadTransaction(
        txId: String
    ): Transaction

    @Throws(NCNativeException::class)
    external fun getMk4Signers(
        records: Array<NdefRecord>
    ): List<SingleSigner>

    @Throws(NCNativeException::class)
    external fun createMk4Signer(
        signer: SingleSigner
    ): SingleSigner

    @Throws(NCNativeException::class)
    external fun exportWalletToMk4(
        walletId: String
    ): Array<NdefRecord>

    @Throws(NCNativeException::class)
    external fun exportWalletToBsmsById(
        walletId: String
    ): String

    @Throws(NCNativeException::class)
    external fun exportWalletToBsms(
        wallet: WalletBridge
    ): String

    @Throws(NCNativeException::class)
    external fun exportPsbtToMk4(
        walletId: String,
        txId: String
    ): Array<NdefRecord>

    @Throws(NCNativeException::class)
    external fun exportRawPsbtToMk4(
        psbt: String
    ): Array<NdefRecord>

    @Throws(NCNativeException::class)
    external fun importTransactionFromMk4(
        walletId: String,
        records: Array<NdefRecord>
    ): Transaction?

    @Throws(NCNativeException::class)
    external fun importWalletFromMk4(
        chain: Int,
        records: Array<NdefRecord>
    ): Wallet?

    @Throws(NCNativeException::class)
    external fun getColdCardWallets(records: Array<NdefRecord>): List<Wallet>?

    @Throws(NCNativeException::class)
    external fun createWallet2(wallet: WalletBridge): Wallet

    @Throws(NCNativeException::class)
    external fun getPrimaryKeyAddress(
        mnemonic: String,
        passphrase: String
    ): String?

    @Throws(NCNativeException::class)
    external fun signLoginMessage(
        mnemonic: String,
        passphrase: String,
        message: String
    ): String?

    @Throws(NCNativeException::class)
    external fun signLoginMessageImpl(
        masterSignerId: String,
        message: String
    ): String?

    @Throws(NCNativeException::class)
    external fun getMasterFingerprint(
        mnemonic: String,
        passphrase: String
    ): String?

    @Throws(NCNativeException::class)
    external fun getPrimaryKeys(chain: Int, storagePath: String): List<PrimaryKey>

    @Throws(NCNativeException::class)
    external fun parseBtcUri(
        uri: String
    ): BtcUri

    @Throws(NCNativeException::class)
    external fun setSelectedWallet(walletId: String): Boolean

    @Throws(NCNativeException::class)
    external fun deletePrimaryKey(): Boolean

    @Throws(NCNativeException::class)
    external fun generateColdCardHealthCheckMessage(
        derivationPath: String,
        message: String?
    ): Array<NdefRecord>

    @Throws(NCNativeException::class)
    external fun healthCheckColdCard(
        signer: SingleSigner,
        records: Array<NdefRecord>
    ): ColdCardHealth

    @Throws(NCNativeException::class)
    external fun isValidDerivationPath(path: String): Boolean

    @Throws(NCNativeException::class)
    external fun getSignerFromMasterSigner(masterSignerId: String, path: String): SingleSigner

    @Throws(NCNativeException::class)
    external fun parseJsonSigners(
        str: String,
        type: SignerType
    ): List<SingleSigner>

    @Throws(NCNativeException::class)
    external fun verifyTapSignerBackup(
        backUpKey: String,
        decryptionKey: String,
        masterSignerId: String
    ): Boolean

    @Throws(NCNativeException::class)
    external fun verifyTapSignerBackupContent(
        content: ByteArray,
        backUpKey: String,
        masterSignerId: String
    ): Boolean

    @Throws(NCNativeException::class)
    external fun hasWallet(
        walletId: String
    ): Boolean

    @Throws(NCNativeException::class)
    external fun addTapSigner(
        cardId: String,
        xfp: String,
        name: String,
        version: String,
        brithHeight: Int,
        isTestNet: Boolean
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
        messagesToSign: String
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
    external fun getDummyTx(walletId: String, message: String): Transaction

    @Throws(NCNativeException::class)
    external fun getDummyTxByByteArray(walletId: String, fileData: ByteArray): Transaction?

    @Throws(NCNativeException::class)
    external fun exportKeystoneDummyTransaction(txToSign: String, density: Int): List<String>

    @Throws(NCNativeException::class)
    external fun parseKeystoneDummyTransaction(qrs: List<String>): String

    @Throws(NCNativeException::class)
    external fun getDummyTransactionSignature(signer: SingleSigner, psbt: String): String

    external fun getColdcardSignatureFromPsbt(
        signer: SingleSigner,
        records: Array<NdefRecord>
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
        vout: Int
    ): Boolean

    @Throws(NCNativeException::class)
    external fun removeFromCoinCollection(
        walletId: String,
        txId: String,
        collectionId: Int,
        vout: Int
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
    external fun updateCoinCollection(walletId: String, coinCollection: CoinCollection): Boolean

    @Throws(NCNativeException::class)
    external fun deleteCoinCollection(walletId: String, collectionId: Int): Boolean

    @Throws(NCNativeException::class)
    external fun isMyAddress(walletId: String, address: String): Boolean

    companion object {
        init {
            System.loadLibrary(LIB_NAME)
        }
    }
}
