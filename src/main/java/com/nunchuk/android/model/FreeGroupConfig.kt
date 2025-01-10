package com.nunchuk.android.model

import android.os.Parcelable
import kotlinx.parcelize.Parcelize

@Parcelize
data class FreeGroupConfig(
    var total: Int,
    var remain: Int,
    var maxKey: Int,
) : Parcelable