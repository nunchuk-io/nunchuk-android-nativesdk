package com.nunchuk.android.type

import androidx.annotation.Keep

// Ordinals must match nunchuk::bitbox::AttestationStatus in libnunchuk (types.hpp).
@Keep
enum class BitBoxAttestationStatus {
    NOT_CHECKED,
    VALID,
    INVALID,
}
