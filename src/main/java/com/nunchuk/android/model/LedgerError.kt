package com.nunchuk.android.model

import androidx.annotation.Keep

// Constructed by JNI (see ledger-jni.cpp). Mirrors nunchuk::ledger::LedgerError.
@Keep
data class LedgerError(
    var code: Int = 0,
    var message: String = "",
    // uint16_t status word from the device, e.g. 0xB008 (SW_INVALID_SIGNATURE_OR_HMAC).
    var statusWord: Int = 0,
)
