package com.nunchuk.android.model

import com.nunchuk.android.type.SignerType

data class MasterSigner(
    var id: String = "",
    var name: String = "",
    var device: Device = Device(),
    var lastHealthCheck: Long = 0,
    var software: Boolean = false,
    var type: SignerType = SignerType.AIRGAP
)