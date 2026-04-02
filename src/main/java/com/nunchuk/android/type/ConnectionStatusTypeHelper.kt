package com.nunchuk.android.type

object ConnectionStatusTypeHelper {
    @JvmStatic
    fun from(ordinal: Int): ConnectionStatus =
        CollectionUtil.fromOrdinal(ConnectionStatus.entries, ordinal, ConnectionStatus.OFFLINE)
}
