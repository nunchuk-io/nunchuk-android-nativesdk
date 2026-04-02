package com.nunchuk.android.type

object SignerTagHelper {
    @JvmStatic
    fun from(ordinal: Int): SignerTag =
        CollectionUtil.fromOrdinal(SignerTag.entries, ordinal, SignerTag.INHERITANCE)
}
