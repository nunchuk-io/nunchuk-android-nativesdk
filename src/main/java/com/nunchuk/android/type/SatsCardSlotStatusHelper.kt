package com.nunchuk.android.type

object SatsCardSlotStatusHelper {
    @JvmStatic
    fun from(ordinal: Int): SatsCardSlotStatus =
        CollectionUtil.fromOrdinal(SatsCardSlotStatus.entries, ordinal, SatsCardSlotStatus.UNUSED)
}
