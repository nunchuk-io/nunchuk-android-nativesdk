package com.nunchuk.android.type

object GroupSpendingLimitIntervalHelper {
    @JvmStatic
    fun from(ordinal: Int): GroupSpendingLimitInterval =
        CollectionUtil.fromOrdinal(GroupSpendingLimitInterval.entries, ordinal, GroupSpendingLimitInterval.DAILY)
}
