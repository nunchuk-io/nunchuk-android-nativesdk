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
import com.nunchuk.android.type.CoinStatus
import kotlinx.parcelize.Parcelize

@Parcelize
data class UnspentOutput(
    var txid: String = "",
    var address: String = "",
    var vout: Int = 0,
    var amount: Amount = Amount.ZER0,
    var height: Int = 0,
    var memo: String = "",
    var isChange: Boolean = false,
    var isLocked: Boolean = false,
    var tags: Set<Int> = emptySet(),
    var collection: Set<Int> = emptySet(),
    var time: Long = 0L,
    var scheduleTime: Long = 0L,
    var status: CoinStatus = CoinStatus.SPENT,
) : Parcelable