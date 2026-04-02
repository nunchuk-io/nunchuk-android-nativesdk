package com.nunchuk.android.type;

public final class GroupDummyTransactionStatusHelper {

    private GroupDummyTransactionStatusHelper() {
    }

    public static GroupDummyTransactionStatus from(final int ordinal) {
        return CollectionUtil.fromOrdinal(GroupDummyTransactionStatus.values(),
                ordinal,
                GroupDummyTransactionStatus.PENDING_SIGNATURES
        );
    }

}
