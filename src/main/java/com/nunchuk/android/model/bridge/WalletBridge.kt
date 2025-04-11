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

package com.nunchuk.android.model.bridge

import com.nunchuk.android.model.Amount
import com.nunchuk.android.model.SingleSigner
import com.nunchuk.android.model.Wallet

data class WalletBridge(
    var id: String = "",
    var name: String = "",
    var totalRequireSigns: Int = 0,
    var signers: List<SingleSigner> = emptyList(),
    var addressType: Int,
    var escrow: Boolean = false,
    var balance: Amount = Amount.ZER0,
    var createDate: Long = 0L,
    var description: String = "",
    var gapLimit: Int = 0,
    var needBackup: Boolean = false,
    var archived: Boolean = false
)

internal fun Wallet.toBridge() = WalletBridge(
    id = id,
    name = name,
    totalRequireSigns = totalRequireSigns,
    signers = signers,
    addressType = addressType.ordinal,
    escrow = escrow,
    balance = balance,
    createDate = createDate,
    description = description,
    gapLimit = gapLimit,
    needBackup = needBackup,
    archived = archived
)