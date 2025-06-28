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
    var replacedTxid: String = "",
    var fee: Amount = Amount.ZER0,
    var feeRate: Amount = Amount.ZER0,
    var blockTime: Long = 0L,
    var subtractFeeFromAmount: Boolean = false,
    var isReceive: Boolean = false,
    var subAmount: Amount = Amount.ZER0,
    var totalAmount: Amount = Amount.ZER0,
    var psbt: String = "",
    var cpfpFee: Amount = Amount.ZER0,
    var keySetStatus: List<KeySetStatus> = emptyList(),
    var scriptPathFee: Amount = Amount.ZER0,
    var lockTime: Long = 0L
) : Parcelable
