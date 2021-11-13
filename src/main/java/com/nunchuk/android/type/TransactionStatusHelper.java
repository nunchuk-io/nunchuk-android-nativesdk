package com.nunchuk.android.type;

// This class should be kept in java since jni can't access object class
public final class TransactionStatusHelper {

    private TransactionStatusHelper() {
    }

    public static TransactionStatus from(final int ordinal) {
        return CollectionUtil.fromOrdinal(TransactionStatus.values(),
                ordinal,
                TransactionStatus.PENDING_SIGNATURES
        );
    }

}