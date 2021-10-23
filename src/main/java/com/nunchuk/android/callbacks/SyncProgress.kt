package com.nunchuk.android.callbacks

interface SyncProgress {
    fun onSyncProgress(finished: Boolean, progress: Int)
}