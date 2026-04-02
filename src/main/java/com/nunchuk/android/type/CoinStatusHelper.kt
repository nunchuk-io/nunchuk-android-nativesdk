package com.nunchuk.android.type

object CoinStatusHelper {
    @JvmStatic
    fun from(ordinal: Int): CoinStatus =
        CollectionUtil.fromOrdinal(CoinStatus.entries, ordinal, CoinStatus.SPENT)
}
