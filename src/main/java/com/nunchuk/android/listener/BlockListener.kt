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

import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.Job
import kotlinx.coroutines.flow.MutableSharedFlow
import kotlinx.coroutines.flow.asSharedFlow
import kotlinx.coroutines.launch

object BlockListener {
    private val appScope = CoroutineScope(Job() + Dispatchers.Main.immediate)
    private val blockChainFlow = MutableSharedFlow<BtcBlockInfo>()

    @JvmStatic
    fun onBlockUpdate(height: Int, hexHeader: String) {
        appScope.launch {
            blockChainFlow.emit(BtcBlockInfo(height, hexHeader))
        }
    }

    fun getBlockChainFlow() = blockChainFlow.asSharedFlow()
}

data class BtcBlockInfo(val height: Int, val hexHeader: String)