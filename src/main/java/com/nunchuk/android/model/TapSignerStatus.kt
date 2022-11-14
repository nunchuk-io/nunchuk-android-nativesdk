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
import kotlinx.parcelize.Parcelize

@Parcelize
data class TapSignerStatus(
    override var isNeedSetup: Boolean = false,
    var isCreateSigner: Boolean = false,
    var backupKey: ByteArray? = null,
    var masterSignerId: String? = null,
    override var ident: String? = null,
    override var authDelayInSecond: Int = 0
) : CardStatus(), Parcelable {

    override fun equals(other: Any?): Boolean {
        if (this === other) return true
        if (other !is TapSignerStatus) return false

        if (isNeedSetup != other.isNeedSetup) return false
        if (isCreateSigner != other.isCreateSigner) return false
        if (backupKey != null) {
            if (other.backupKey == null) return false
            if (!backupKey.contentEquals(other.backupKey)) return false
        } else if (other.backupKey != null) return false
        if (ident != other.ident) return false
        if (authDelayInSecond != other.authDelayInSecond) return false

        return true
    }

    override fun hashCode(): Int {
        var result = isNeedSetup.hashCode()
        result = 31 * result + isCreateSigner.hashCode()
        result = 31 * result + (backupKey?.contentHashCode() ?: 0)
        result = 31 * result + (ident?.hashCode() ?: 0)
        result = 31 * result + authDelayInSecond
        return result
    }
}
