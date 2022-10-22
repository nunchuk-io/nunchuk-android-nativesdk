package com.nunchuk.android.model

import android.os.Parcelable
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
    var descriptor: String = ""
) : Parcelable