package com.nunchuk.android.type

object WalletTypeHelper {
    @JvmStatic
    fun from(ordinal: Int): WalletType =
        CollectionUtil.fromOrdinal(WalletType.entries, ordinal, WalletType.MULTI_SIG)
}
