package com.nunchuk.android.type;

public final class GroupWalletAlertTypeHelper {

    private GroupWalletAlertTypeHelper() {
    }

    public static GroupWalletAlertType from(final int ordinal) {
        return CollectionUtil.fromOrdinal(GroupWalletAlertType.values(),
                ordinal,
                GroupWalletAlertType.UNKNOWN
        );
    }

}
