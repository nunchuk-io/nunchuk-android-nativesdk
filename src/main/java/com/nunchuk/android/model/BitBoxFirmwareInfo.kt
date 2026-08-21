package com.nunchuk.android.model

import androidx.annotation.Keep
import com.nunchuk.android.type.BitBoxProduct

/**
 * Constructed by JNI (see bitbox-jni.cpp). Mirrors nunchuk::bitbox::BitBoxFirmwareInfo —
 * what `InspectFirmware` reads out of a signed firmware file before an upgrade.
 *
 * The [bitBoxProduct] must match the connected device's product or the upgrade must not
 * be started.
 */
@Keep
data class BitBoxFirmwareInfo(
    var product: Int = BitBoxProduct.UNKNOWN.ordinal,
    var monotonicVersion: Long = 0L,
    var firmwareSize: Long = 0L,
) {
    val bitBoxProduct: BitBoxProduct
        get() = BitBoxProduct.entries.getOrElse(product) { BitBoxProduct.UNKNOWN }
}
