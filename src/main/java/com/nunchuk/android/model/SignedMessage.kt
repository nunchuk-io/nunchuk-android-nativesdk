package com.nunchuk.android.model

data class SignedMessage(
    var address: String = "",
    var signature: String = "",
    var rfc2440: String = "",
)