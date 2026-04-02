package com.nunchuk.android.type

object GroupWalletAlertTypeHelper {
    @JvmStatic
    fun from(ordinal: Int): GroupWalletAlertType =
        CollectionUtil.fromOrdinal(GroupWalletAlertType.entries, ordinal, GroupWalletAlertType.UNKNOWN)
}
