package com.nunchuk.android.callbacks

interface UploadFileCallBack {
    fun onUpload(fileName: String, mineType: String, fileJsonInfo: String, dataLength: Int)
}