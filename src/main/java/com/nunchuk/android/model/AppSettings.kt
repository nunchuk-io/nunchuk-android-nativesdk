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

import com.nunchuk.android.type.BackendType
import com.nunchuk.android.type.Chain

data class AppSettings(
        val chain: Chain = Chain.TESTNET,
        val backendType: BackendType = BackendType.ELECTRUM,
        val mainnetServers: List<String> = emptyList(),
        val testnetServers: List<String> = emptyList(),
        val signetServers: List<String> = emptyList(),
        val hwiPath: String = "",
        val storagePath: String = "",
        val enableProxy: Boolean = false,
        val proxyHost: String = "",
        val proxyPort: Int = 0,
        val proxyUsername: String = "",
        val proxyPassword: String = "",
        val certificateFile: String = "",
        val coreRpcHost: String = "",
        val coreRpcPort: Int = 0,
        val coreRpcUsername: String = "",
        val coreRpcPassword: String = "",
        val signetExplorerHost: String = "",
        val testExplorerHost: String = "",
        val mainnetExplorerHost: String = "",
)