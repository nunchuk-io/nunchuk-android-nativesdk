package com.nunchuk.android.type

import androidx.annotation.Keep

// Ordinals must match nunchuk::ledger::LedgerTransport in libnunchuk (types.hpp).
@Keep
enum class LedgerTransport {
    BLE,
    USB_HID,
}
