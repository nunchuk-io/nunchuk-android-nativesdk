package com.nunchuk.android.model

import android.os.Parcelable
import kotlinx.parcelize.Parcelize

@Parcelize
data class TxInput(
    var first: String = "",
    var second: Int = 0
) : Parcelable

@Parcelize
data class TxOutput(
    var first: String = "",
    var second: Amount = Amount.ZER0
) : Parcelable