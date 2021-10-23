package com.nunchuk.android.model;

import com.nunchuk.android.callbacks.UploadFileCallBack;

public final class SyncFileEventHelper {

    public static UploadFileCallBack executor = (fileName, mineType, fileJsonInfo, dataLength) -> {};

    private SyncFileEventHelper() {
    }

    public static void sendEvent(final String fileName, final String mineType, final String fileJsonInfo, final int dataLength) {
        executor.onUpload(fileName, mineType, fileJsonInfo, dataLength);
    }
}
