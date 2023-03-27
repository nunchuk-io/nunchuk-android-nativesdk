package com.nunchuk.android.model

import android.os.Parcelable
import kotlinx.parcelize.Parcelize

@Parcelize
data class CoinCollection(var id: Int = -1, var name: String = "") : Parcelable