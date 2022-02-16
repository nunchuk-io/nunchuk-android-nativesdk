package com.nunchuk.android.type;

// This class should be kept in java since jni can't access object class
public final class HealthStatusTypeHelper {

    private HealthStatusTypeHelper() {
    }

    public static HealthStatus from(final int ordinal) {
        return CollectionUtil.fromOrdinal(HealthStatus.values(),
                ordinal,
                HealthStatus.SIGNATURE_INVALID
        );
    }

}