package com.nunchuk.android.type;

// This class should be kept in java since jni can't access object class
public final class ConnectionStatusTypeHelper {

    private ConnectionStatusTypeHelper() {
    }

    public static ConnectionStatus from(final int ordinal) {
        return CollectionUtil.fromOrdinal(ConnectionStatus.values(),
                ordinal,
                ConnectionStatus.OFFLINE
        );
    }

}