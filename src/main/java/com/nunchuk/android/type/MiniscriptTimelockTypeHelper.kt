package com.nunchuk.android.type

object MiniscriptTimelockTypeHelper {
    @JvmStatic
    fun from(ordinal: Int): MiniscriptTimelockType =
        CollectionUtil.fromOrdinal(MiniscriptTimelockType.entries, ordinal, MiniscriptTimelockType.ABSOLUTE)
}
