package com.nunchuk.android.model

import android.os.Parcelable
import kotlinx.parcelize.Parcelize

@Parcelize
data class Amount(var value: Long = 0, var formattedValue: String = "0.00") : Parcelable {

    companion object {
        val ZER0 = Amount(0, "0.00")
    }

    operator fun plus(amount: Amount) = Amount(value + amount.value)
    operator fun plusAssign(amount: Amount) {
        value += amount.value
    }
    operator fun minus(amount: Amount) = Amount(value - amount.value)
}