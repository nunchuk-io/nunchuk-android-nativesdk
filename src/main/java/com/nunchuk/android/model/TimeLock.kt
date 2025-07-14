package com.nunchuk.android.model

import com.nunchuk.android.type.MiniscriptTimelockBased
import com.nunchuk.android.type.MiniscriptTimelockType

data class TimeLock(
    val based: MiniscriptTimelockBased,
    val type: MiniscriptTimelockType,
    val value: Long
) {
    // Default constructor for JNI
    constructor() : this(MiniscriptTimelockBased.NONE, MiniscriptTimelockType.ABSOLUTE, 0L)
} 