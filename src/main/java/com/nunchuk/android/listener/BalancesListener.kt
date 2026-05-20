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

package com.nunchuk.android.listener

import com.nunchuk.android.model.Amount
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.Job
import kotlinx.coroutines.flow.MutableSharedFlow
import kotlinx.coroutines.flow.asSharedFlow
import kotlinx.coroutines.launch

object BalancesListener {
    private val appScope = CoroutineScope(Job() + Dispatchers.IO)
    private val internalBalancesUpdateFlow = MutableSharedFlow<BalancesUpdate>()
    val balancesUpdateFlow = internalBalancesUpdateFlow.asSharedFlow()

    @JvmStatic
    fun onBalancesUpdate(
        walletId: String,
        balance: Long,
        unconfirmedBalance: Long,
        assetBalances: Map<String, Long>,
    ) {
        appScope.launch {
            internalBalancesUpdateFlow.emit(
                BalancesUpdate(
                    walletId = walletId,
                    balance = Amount(value = balance),
                    unconfirmedBalance = Amount(value = unconfirmedBalance),
                    assetBalances = assetBalances.mapValues { Amount(value = it.value) },
                )
            )
        }
    }
}

data class BalancesUpdate(
    val walletId: String,
    val balance: Amount,
    val unconfirmedBalance: Amount,
    val assetBalances: Map<String, Amount>,
)
