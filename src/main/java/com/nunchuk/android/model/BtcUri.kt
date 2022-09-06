package com.nunchuk.android.model

data class BtcUri(
    var address: String = "",
    var amount: Amount = Amount(0),
    var label: String = "",
    var message: String = ""
)