package com.nunchuk.android.type

object TransactionStatusHelper {
    @JvmStatic
    fun from(ordinal: Int): TransactionStatus =
        CollectionUtil.fromOrdinal(TransactionStatus.entries, ordinal, TransactionStatus.PENDING_SIGNATURES)
}
