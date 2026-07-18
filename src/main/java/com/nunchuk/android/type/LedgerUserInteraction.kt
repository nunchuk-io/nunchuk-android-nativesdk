package com.nunchuk.android.type

import androidx.annotation.Keep

// Ordinals must match nunchuk::ledger::UserInteraction in libnunchuk (types.hpp).
@Keep
enum class LedgerUserInteraction {
    NONE,
    UNLOCK_DEVICE,
    CONFIRM_OPEN_APP,
    VERIFY_ADDRESS,
    REGISTER_WALLET,
    SIGN_MESSAGE,
    SIGN_TRANSACTION,
}
