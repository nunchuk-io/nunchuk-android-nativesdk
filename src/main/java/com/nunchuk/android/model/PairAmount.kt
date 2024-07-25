package com.nunchuk.android.model

import android.os.Parcelable
import kotlinx.parcelize.Parcelize

@Parcelize
data class PairAmount(
    var first: Amount = Amount.ZER0,
    var second: Amount = Amount.ZER0
) : Parcelable