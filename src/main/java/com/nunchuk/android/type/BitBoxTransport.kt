package com.nunchuk.android.type

import androidx.annotation.Keep

// Ordinals must match nunchuk::bitbox::BitBoxTransport in libnunchuk (types.hpp).
@Keep
enum class BitBoxTransport {
    BLE,
    USB_HID,
}
