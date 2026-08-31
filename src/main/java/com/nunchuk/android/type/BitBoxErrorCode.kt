package com.nunchuk.android.type

import androidx.annotation.Keep

/**
 * Mirrors nunchuk::bitbox::BitBoxErrorCode in libnunchuk (types.hpp).
 *
 * Unlike the other BitBox enums these are **explicit values, not ordinals** — the 1xx
 * block is returned directly by the BitBox firmware protobuf API — so map with
 * [from] rather than by index.
 */
@Keep
enum class BitBoxErrorCode(val value: Int) {
    NONE(0),
    INVALID_STATE(1),
    INVALID_ARGUMENT(2),
    INVALID_RESPONSE(3),
    PROTOCOL(5),
    PAIRING_REJECTED(6),
    UNSUPPORTED_DEVICE(8),
    UNSUPPORTED_FIRMWARE(9),
    UNSUPPORTED_WALLET(10),
    INVALID_PSBT(11),
    STORAGE(12),
    ATTESTATION(13),
    ANTI_KLEPTO(14),
    SESSION_LOST(15),
    DEVICE_UNINITIALIZED(17),
    INVALID_FIRMWARE(18),

    // Values returned directly by the BitBox firmware protobuf API.
    DEVICE_INVALID_INPUT(101),
    DEVICE_MEMORY(102),
    DEVICE(103),
    USER_ABORT(104),
    DEVICE_INVALID_STATE(105),
    DEVICE_DISABLED(106),
    DEVICE_DUPLICATE(107),
    DEVICE_NOISE_ENCRYPT(108),
    DEVICE_NOISE_DECRYPT(109),
    ;

    companion object {
        fun from(value: Int): BitBoxErrorCode = entries.firstOrNull { it.value == value } ?: NONE
    }
}
