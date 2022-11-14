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

class StorageException(override val code: Int) : BaseException(code) {

    companion object {
        const val WALLET_NOT_FOUND = -2001
        const val MASTERSIGNER_NOT_FOUND = -2002
        const val TX_NOT_FOUND = -2003
        const val SIGNER_USED = -2005
        const val INVALID_DATADIR = -2006
        const val SQL_ERROR = -2007
        const val WALLET_EXISTED = -2008
        const val SIGNER_EXISTS = -2009
        const val SIGNER_NOT_FOUND = -2010
        const val ADDRESS_NOT_FOUND = -2011
    }

}