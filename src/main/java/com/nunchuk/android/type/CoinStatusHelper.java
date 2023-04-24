package com.nunchuk.android.type;

// This class should be kept in java since jni can't access object class
public final class CoinStatusHelper {

    private CoinStatusHelper() {
    }

    public static CoinStatus from(final int ordinal) {
        return CollectionUtil.fromOrdinal(CoinStatus.values(),
                ordinal,
                CoinStatus.SPENT
        );
    }

}