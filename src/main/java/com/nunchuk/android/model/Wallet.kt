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
import com.nunchuk.android.type.AddressType
import com.nunchuk.android.type.WalletTemplate
import kotlinx.parcelize.Parcelize

@Parcelize
data class Wallet(
    var id: String = "",
    var name: String = "",
    var totalRequireSigns: Int = 0,
    var signers: List<SingleSigner> = emptyList(),
    var addressType: AddressType = AddressType.NESTED_SEGWIT,
    var escrow: Boolean = false,
    var balance: Amount = Amount.ZER0,
    var createDate: Long = 0L,
    var description: String = "",
    var gapLimit: Int = 0,
    var needBackup: Boolean = false,
    var walletTemplate: WalletTemplate = WalletTemplate.DEFAULT,
    var archived: Boolean = false
) : Parcelable {
    companion object {
        val EMPTY = Wallet()
    }
}