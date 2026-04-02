package com.nunchuk.android.type

object MiniscriptTimelockBasedHelper {
    @JvmStatic
    fun from(ordinal: Int): MiniscriptTimelockBased =
        CollectionUtil.fromOrdinal(MiniscriptTimelockBased.entries, ordinal, MiniscriptTimelockBased.NONE)
}
