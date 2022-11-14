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

package com.nunchuk.android.exception

class RPCException(override val code: Int) : BaseException(code) {

    companion object {
        const val RPC_MISC_ERROR = -3001
        const val RPC_TYPE_ERROR = -3003
        const val RPC_WALLET_EXISTS = -3004
        const val RPC_INVALID_ADDRESS_OR_KEY = -3005
        const val RPC_OUT_OF_MEMORY = -3007
        const val RPC_INVALID_PARAMETER = -3008
        const val RPC_WALLET_NOT_FOUND = -3018
        const val RPC_DATABASE_ERROR = -3020
        const val RPC_DESERIALIZATION_ERROR = -3022
        const val RPC_VERIFY_ERROR = -3025
        const val RPC_VERIFY_REJECTED = -3026
        const val RPC_VERIFY_ALREADY_IN_CHAIN = -3027
        const val RPC_IN_WARMUP = -3028
        const val RPC_METHOD_DEPRECATED = -3032
        // Nunchuk-defined error codes
        const val RPC_REQUEST_ERROR = -3099
    }

}