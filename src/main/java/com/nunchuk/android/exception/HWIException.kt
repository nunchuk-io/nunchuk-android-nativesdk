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

class HWIException(override val code: Int) : BaseException(code) {

    companion object {
        const val NO_DEVICE_TYPE = -4001
        const val MISSING_ARGUMENTS = -4002
        const val DEVICE_CONN_ERROR = -4003
        const val UNKNOWN_DEVICE_TYPE = -4004
        const val INVALID_TX = -4005
        const val NO_PASSWORD = -4006
        const val BAD_ARGUMENT = -4007
        const val NOT_IMPLEMENTED = -4008
        const val UNAVAILABLE_ACTION = -4009
        const val DEVICE_ALREADY_INIT = -4010
        const val DEVICE_ALREADY_UNLOCKED = -4011
        const val DEVICE_NOT_READY = -4012
        const val UNKNOWN_ERROR = -4013
        const val ACTION_CANCELED = -4014
        const val DEVICE_BUSY = -4015
        const val NEED_TO_BE_ROOT = -4016
        const val HELP_TEXT = -4017
        const val DEVICE_NOT_INITIALIZED = -4018

        // Nunchuk-defined error codes
        const val RUN_ERROR = -4099
        const val INVALID_RESULT = -4098
    }

}