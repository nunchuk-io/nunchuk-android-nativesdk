package com.nunchuk.android.model

import android.os.Parcelable
import kotlinx.parcelize.Parcelize

@Parcelize
data class Device(
    var type: String = "",
    var model: String = "",
    var path: String = "",
    var masterFingerprint: String = "",
    var connected: Boolean = false,
    var needPassPhraseSent: Boolean = false,
    var needPinSet: Boolean = false,
    var initialized: Boolean = true,
    var isTapsigner: Boolean = false
) : Parcelable


