package com.nunchuk.android.callbacks

interface UploadFileCallBack {
    fun onUpload(fileName: String, mineType: String, fileJsonInfo: String, data: ByteArray, dataLength: Int)
}