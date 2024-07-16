package com.nunchuk.android.type

import androidx.annotation.Keep

@Keep
enum class SignerType {
    UNKNOWN,
    HARDWARE,
    AIRGAP,
    SOFTWARE,
    FOREIGN_SOFTWARE,
    NFC,
    COLDCARD_NFC,
    SERVER,
    PORTAL_NFC
}