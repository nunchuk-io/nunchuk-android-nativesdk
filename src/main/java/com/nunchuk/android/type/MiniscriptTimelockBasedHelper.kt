package com.nunchuk.android.type;

public final class MiniscriptTimelockBasedHelper {
    private MiniscriptTimelockBasedHelper() {}

    public static MiniscriptTimelockBased from(final int ordinal) {
        return CollectionUtil.fromOrdinal(MiniscriptTimelockBased.values(), ordinal, MiniscriptTimelockBased.NONE);
    }
} 