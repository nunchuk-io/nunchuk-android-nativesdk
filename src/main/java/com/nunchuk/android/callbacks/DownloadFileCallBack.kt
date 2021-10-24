package com.nunchuk.android.callbacks

interface DownloadFileCallBack {
    fun onDownload(fileName: String, mineType: String, fileJsonInfo: String, fileUrl: String)
}