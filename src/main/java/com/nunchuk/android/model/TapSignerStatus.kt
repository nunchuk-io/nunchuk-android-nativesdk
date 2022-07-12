package com.nunchuk.android.model

class TapSignerStatus(
    var isSetup: Boolean = false,
    var isCreateSigner: Boolean = false,
    var backupKey: ByteArray? = null,
    var ident: String? = null,
    var authDelayInSecond: Int = 0
)
