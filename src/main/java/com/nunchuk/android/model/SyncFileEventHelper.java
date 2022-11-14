/**************************************************************************
 * This file is part of the Nunchuk software (https://nunchuk.io/)        *							          *
 * Copyright (C) 2022 Nunchuk								              *
 *                                                                        *
 * This program is free software; you can redistribute it and/or          *
 * modify it under the terms of the GNU General Public License            *
 * as published by the Free Software Foundation; either version 3         *
 * of the License, or (at your option) any later version.                 *
 *                                                                        *
 * This program is distributed in the hope that it will be useful,        *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 * GNU General Public License for more details.                           *
 *                                                                        *
 * You should have received a copy of the GNU General Public License      *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
 *                                                                        *
 **************************************************************************/

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
