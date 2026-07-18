package com.nunchuk.android.type

import androidx.annotation.Keep

// Ordinals must match nunchuk::ledger::LedgerStepType in libnunchuk (types.hpp).
@Keep
enum class LedgerStepType {
    WRITE,
    READ_MORE,
    COMPLETE,
    FAILED,
    APP_SWITCH,
}
