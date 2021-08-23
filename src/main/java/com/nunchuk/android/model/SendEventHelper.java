package com.nunchuk.android.model;

public final class SendEventHelper {

    public static SendEventExecutor executor;

    public static void sendEvent(final String roomId, final String type, final String content) {
        executor.execute(roomId, type, content);
    }

}
