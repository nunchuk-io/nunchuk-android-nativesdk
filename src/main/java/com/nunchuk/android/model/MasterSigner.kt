package com.nunchuk.android.model

import android.os.Parcelable
import com.nunchuk.android.type.SignerType
import kotlinx.parcelize.Parcelize

@Parcelize
data class MasterSigner(
    var id: String = "",
    var name: String = "",
    var device: Device = Device(),
    var lastHealthCheck: Long = 0,
    var software: Boolean = false,
    var type: SignerType = SignerType.AIRGAP
) : Parcelable