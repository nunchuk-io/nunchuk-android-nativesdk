package com.nunchuk.android.model

import android.os.Parcelable
import kotlinx.parcelize.Parcelize

@Parcelize
data class CoinCollection(
    var id: Int = -1,
    var name: String = "",
    var isAddNewCoin: Boolean = false,
    var isAutoLock: Boolean = false,
    var tagIds: List<Int> = emptyList(),
) : Parcelable