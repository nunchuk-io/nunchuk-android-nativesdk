package com.nunchuk.android.model;

import com.nunchuk.android.callbacks.DownloadFileCallBack;
import com.nunchuk.android.callbacks.SyncFileCallBack;
import com.nunchuk.android.callbacks.UploadFileCallBack;

public final class SyncFileEventHelper {

    public static UploadFileCallBack executor = (fileName, mineType, fileJsonInfo, data, dataLength) -> {};

    public static DownloadFileCallBack downloadFileExecutor = (fileName, mineType, fileJsonInfo, fileUrl) -> {};

    public static SyncFileCallBack syncFileExecutor = (finished, progress) -> {};

    private SyncFileEventHelper() {
    }

    public static void uploadFileEvent(final String fileName, final String mineType, final String fileJsonInfo, final byte[] data, final int dataLength) {
        executor.onUpload(fileName, mineType, fileJsonInfo, data, dataLength);
    }

    public static void downloadFileEvent(final String fileName, final String mineType, final String fileJsonInfo, final String fileUrl) {
        downloadFileExecutor.onDownload(fileName, mineType, fileJsonInfo, fileUrl);
    }

    public static void syncFileEvent(final boolean finished, final int progress) {
        syncFileExecutor.onSync(finished, progress);
    }
}
