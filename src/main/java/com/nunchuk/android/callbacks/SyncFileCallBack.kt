package com.nunchuk.android.callbacks

interface SyncFileCallBack {
    fun onSync(finished: Boolean, progress: Int)
}