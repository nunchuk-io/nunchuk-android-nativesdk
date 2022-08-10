package com.nunchuk.android.model

import android.os.Parcelable
import com.nunchuk.android.type.TransactionStatus
import kotlinx.parcelize.Parcelize

@Parcelize
data class Transaction(
    var txId: String = "",
    var height: Int = 0,
    var inputs: List<TxInput> = emptyList(),
    var outputs: List<TxOutput> = emptyList(),
    var userOutputs: List<TxOutput> = emptyList(),
    var receiveOutputs: List<TxOutput> = emptyList(),
    var changeIndex: Int = 0,
    var m: Int = 0,
    var signers: Map<String, Boolean> = emptyMap(),
    var memo: String = "",
    var status: TransactionStatus = TransactionStatus.PENDING_SIGNATURES,
    var replacedByTxid: String = "",
    var fee: Amount = Amount.ZER0,
    var feeRate: Amount = Amount.ZER0,
    var blockTime: Long = 0L,
    var subtractFeeFromAmount: Boolean = false,
    var isReceive: Boolean = false,
    var subAmount: Amount = Amount.ZER0,
    var totalAmount: Amount = Amount.ZER0
) : Parcelable
