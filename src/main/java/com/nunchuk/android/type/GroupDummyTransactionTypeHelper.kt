package com.nunchuk.android.type

object GroupDummyTransactionTypeHelper {
    @JvmStatic
    fun from(ordinal: Int): GroupDummyTransactionType =
        CollectionUtil.fromOrdinal(GroupDummyTransactionType.entries, ordinal, GroupDummyTransactionType.UNKNOWN)
}
