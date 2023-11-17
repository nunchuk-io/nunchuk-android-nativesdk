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
import com.nunchuk.android.type.SignerTag
import com.nunchuk.android.type.SignerType
import kotlinx.parcelize.Parcelize

@Parcelize
data class SingleSigner(
    var name: String = "",
    var xpub: String = "",
    var publicKey: String = "",
    var derivationPath: String = "",
    var masterFingerprint: String = "",
    var lastHealthCheck: Long = 0L,
    var masterSignerId: String = "",
    var used: Boolean = false,
    var type: SignerType = SignerType.AIRGAP,
    var hasMasterSigner: Boolean = false,
    var descriptor: String = "",
    var tags: List<SignerTag> = emptyList(),
    var isVisible: Boolean = false,
    var index: Int = 0,
) : Parcelable