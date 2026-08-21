package com.nunchuk.android.model

import androidx.annotation.Keep
import com.nunchuk.android.type.BitBoxProduct

/**
 * Constructed by JNI (see bitbox-jni.cpp). Mirrors nunchuk::bitbox::BitBoxDeviceInfo.
 *
 * `product` is a [BitBoxProduct] ordinal; read it via [bitBoxProduct].
 */
@Keep
data class BitBoxDeviceInfo(
    var product: Int = BitBoxProduct.UNKNOWN.ordinal,
    var firmwareVersion: String = "",
    var firmwareUpgradeRequired: Boolean = false,
    var name: String = "",
    var unlocked: Boolean = false,
    var initialized: Boolean = false,
    var mnemonicPassphraseEnabled: Boolean = false,
    var securechipModel: String = "",
    var passwordStretchingAlgo: String = "",
    var bluetoothEnabled: Boolean = false,
    var bluetoothFirmwareVersion: String = "",
    var bluetoothFirmwareHash: String = "",
) {
    val bitBoxProduct: BitBoxProduct
        get() = BitBoxProduct.entries.getOrElse(product) { BitBoxProduct.UNKNOWN }
}
