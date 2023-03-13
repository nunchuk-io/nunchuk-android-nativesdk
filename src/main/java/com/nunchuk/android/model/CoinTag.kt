package com.nunchuk.android.model

import android.os.Parcelable
import kotlinx.parcelize.Parcelize

@Parcelize
data class CoinTag(var id: Int = 0, var name: String = "", var color: String = "") : Parcelable