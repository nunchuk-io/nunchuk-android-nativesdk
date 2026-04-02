package com.nunchuk.android.type

object GroupDummyTransactionStatusHelper {
    @JvmStatic
    fun from(ordinal: Int): GroupDummyTransactionStatus =
        CollectionUtil.fromOrdinal(GroupDummyTransactionStatus.entries, ordinal, GroupDummyTransactionStatus.PENDING_SIGNATURES)
}
