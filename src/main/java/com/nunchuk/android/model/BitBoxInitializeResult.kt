package com.nunchuk.android.model

import androidx.annotation.Keep
import com.nunchuk.android.type.BitBoxAttestationStatus

/**
 * Constructed by JNI (see bitbox-jni.cpp). Mirrors nunchuk::bitbox::InitializeResult —
 * the result of the `initialize()` operation.
 *
 * Check in this order: [isAttestationInvalid] (genuine-device check failed — refuse to
 * use the device), then `device.firmwareUpgradeRequired` (upgrade-only), then
 * `device.initialized` (needs setup).
 */
@Keep
data class BitBoxInitializeResult(
    var device: BitBoxDeviceInfo = BitBoxDeviceInfo(),
    var attestation: Int = BitBoxAttestationStatus.NOT_CHECKED.ordinal,
    var attestationMessage: String = "",
) {
    val attestationStatus: BitBoxAttestationStatus
        get() = BitBoxAttestationStatus.entries.getOrElse(attestation) {
            BitBoxAttestationStatus.NOT_CHECKED
        }

    val isAttestationInvalid: Boolean
        get() = attestationStatus == BitBoxAttestationStatus.INVALID
}
