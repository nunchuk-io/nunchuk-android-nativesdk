package com.nunchuk.android.model

import com.nunchuk.android.type.HealthStatus

class ColdCardHealth(
    var status: HealthStatus = HealthStatus.SIGNATURE_INVALID,
    var signature: String = "",
)