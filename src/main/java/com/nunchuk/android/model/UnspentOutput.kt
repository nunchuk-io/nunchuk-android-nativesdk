package com.nunchuk.android.model

import android.os.Parcelable
import kotlinx.parcelize.Parcelize

@Parcelize
data class UnspentOutput(
    var txid: String = "",
    var vout: Int = 0,
    var amount: Amount = Amount.ZER0,
    var height: Int = 0,
    var memo: String = ""
) : Parcelable
