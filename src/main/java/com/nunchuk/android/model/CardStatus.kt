package com.nunchuk.android.model

abstract class CardStatus(
    open var ident: String? = null,
    open var isNeedSetup: Boolean = false,
    open var authDelayInSecond: Int = 0,
)