package com.nunchuk.android.type

object HealthStatusTypeHelper {
    @JvmStatic
    fun from(ordinal: Int): HealthStatus =
        CollectionUtil.fromOrdinal(HealthStatus.entries, ordinal, HealthStatus.SIGNATURE_INVALID)
}
