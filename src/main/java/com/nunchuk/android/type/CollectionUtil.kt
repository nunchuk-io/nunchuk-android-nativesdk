package com.nunchuk.android.type

object CollectionUtil {

    @JvmStatic
    fun <T : Enum<T>> fromOrdinal(
        values: Array<T>,
        ordinal: Int,
        defaultValue: T
    ) = values.firstOrNull { it.ordinal == ordinal } ?: defaultValue

}
