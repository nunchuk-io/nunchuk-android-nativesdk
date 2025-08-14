package com.nunchuk.android.model

import android.os.Parcelable
import com.nunchuk.android.type.MiniscriptTimelockBased
import com.nunchuk.android.type.MiniscriptTimelockType
import kotlinx.parcelize.Parcelize

@Parcelize
data class TimeLock(
    val based: MiniscriptTimelockBased,
    val type: MiniscriptTimelockType,
    val value: Long
) : Parcelable {
    // Default constructor for JNI
    constructor() : this(MiniscriptTimelockBased.NONE, MiniscriptTimelockType.ABSOLUTE, 0L)
} 