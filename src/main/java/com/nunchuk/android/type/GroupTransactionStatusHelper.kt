package com.nunchuk.android.type

object GroupTransactionStatusHelper {
    @JvmStatic
    fun from(ordinal: Int): GroupTransactionStatus =
        CollectionUtil.fromOrdinal(GroupTransactionStatus.entries, ordinal, GroupTransactionStatus.UNKNOWN)
}
