package com.nunchuk.android.model

import android.os.Parcelable
import kotlinx.parcelize.Parcelize

@Parcelize
data class OccupiedSlot(
    val time: Long,
    val deviceId: String,
) : Parcelable