package com.nunchuk.android.model;

import com.nunchuk.android.type.ConnectionStatusTypeHelper;

public final class ConnectionStatusHelper {

    public static ConnectionStatusExecutor executor = (status, percent) -> {};

    private ConnectionStatusHelper() {}

    public static void addListener(final int status, final int percent) {
        executor.execute(ConnectionStatusTypeHelper.from(status), percent);
    }
}
