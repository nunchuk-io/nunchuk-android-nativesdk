package com.nunchuk.android.model

import android.os.Parcelable
import kotlinx.android.parcel.Parcelize

@Parcelize
data class Amount(var value: Long = 0, var formattedValue: String = "0.00") : Parcelable {

    companion object {
        val ZER0 = Amount(0, "0.00")
    }

}