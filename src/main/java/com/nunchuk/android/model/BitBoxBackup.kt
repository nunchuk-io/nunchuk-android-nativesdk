package com.nunchuk.android.model

import androidx.annotation.Keep

/**
 * Constructed by JNI (see bitbox-jni.cpp). Mirrors nunchuk::bitbox::BitBoxBackup — one
 * entry of the microSD backup list. [timestamp] is unix seconds.
 */
@Keep
data class BitBoxBackup(
    var id: String = "",
    var name: String = "",
    var timestamp: Long = 0L,
)
