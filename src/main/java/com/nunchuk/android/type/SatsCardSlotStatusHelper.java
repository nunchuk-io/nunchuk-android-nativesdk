package com.nunchuk.android.type;

// This class should be kept in java since jni can't access object class
public final class SatsCardSlotStatusHelper {

    private SatsCardSlotStatusHelper() {
    }

    public static SatsCardSlotStatus from(final int ordinal) {
        return CollectionUtil.fromOrdinal(SatsCardSlotStatus.values(),
                ordinal,
                SatsCardSlotStatus.UNUSED
        );
    }

}