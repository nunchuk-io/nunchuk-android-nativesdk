package com.nunchuk.android.type;

public final class GroupDummyTransactionTypeHelper {

    private GroupDummyTransactionTypeHelper() {
    }

    public static GroupDummyTransactionType from(final int ordinal) {
        return CollectionUtil.fromOrdinal(GroupDummyTransactionType.values(),
                ordinal,
                GroupDummyTransactionType.UNKNOWN
        );
    }

}
