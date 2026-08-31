package com.nunchuk.android.model

import androidx.annotation.Keep
import com.nunchuk.android.type.BitBoxErrorCode

/**
 * Constructed by JNI (see bitbox-jni.cpp). Mirrors nunchuk::bitbox::BitBoxError.
 *
 * [code] holds the raw `BitBoxErrorCode` value (not an ordinal — see [BitBoxErrorCode]);
 * read it as an enum via [errorCode]. [deviceCode] is the firmware's own error code and
 * is only meaningful for the `DEVICE_*` codes.
 */
@Keep
data class BitBoxError(
    var code: Int = BitBoxErrorCode.NONE.value,
    var message: String = "",
    var deviceCode: Int = 0,
) {
    val errorCode: BitBoxErrorCode
        get() = BitBoxErrorCode.from(code)
}
