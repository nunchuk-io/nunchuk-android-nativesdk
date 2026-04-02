package com.nunchuk.android.type

object AddressTypeHelper {
    @JvmStatic
    fun from(ordinal: Int): AddressType =
        CollectionUtil.fromOrdinal(AddressType.entries, ordinal, AddressType.NATIVE_SEGWIT)
}
