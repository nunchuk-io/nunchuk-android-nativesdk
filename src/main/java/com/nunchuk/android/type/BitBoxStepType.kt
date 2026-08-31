package com.nunchuk.android.type

import androidx.annotation.Keep

// Ordinals must match nunchuk::bitbox::BitBoxStepType in libnunchuk (types.hpp).
//
// COMPLETE, FAILED and REBOOT are terminal: drive a step to one of them before
// starting another operation.
@Keep
enum class BitBoxStepType {
    WRITE,
    READ_MORE,
    RETRY_AFTER,
    AWAITING_USER,
    COMPLETE,
    FAILED,
    REBOOT,
}
