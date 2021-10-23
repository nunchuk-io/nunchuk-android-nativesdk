package com.nunchuk.android.model;

public final class SendEventHelper {

    public static SendEventExecutor executor = (roomId, type, content) -> "";

    private SendEventHelper() {
    }

    public static void sendEvent(final String roomId, final String type, final String content) {
        executor.execute(roomId, type, content);
    }

}
