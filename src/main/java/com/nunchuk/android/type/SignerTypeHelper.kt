package com.nunchuk.android.type

object SignerTypeHelper {
    @JvmStatic
    fun from(ordinal: Int): SignerType =
        CollectionUtil.fromOrdinal(SignerType.entries, ordinal, SignerType.AIRGAP)
}
