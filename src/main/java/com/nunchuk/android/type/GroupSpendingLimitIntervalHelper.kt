package com.nunchuk.android.type;

public final class GroupSpendingLimitIntervalHelper {

    private GroupSpendingLimitIntervalHelper() {
    }

    public static GroupSpendingLimitInterval from(final int ordinal) {
        return CollectionUtil.fromOrdinal(GroupSpendingLimitInterval.values(),
                ordinal,
                GroupSpendingLimitInterval.DAILY
        );
    }

}
