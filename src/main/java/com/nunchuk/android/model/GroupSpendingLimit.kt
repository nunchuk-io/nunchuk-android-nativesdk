package com.nunchuk.android.model

import android.os.Parcelable
import com.nunchuk.android.type.GroupSpendingLimitInterval
import kotlinx.parcelize.Parcelize

@Parcelize
data class GroupSpendingLimit(
    val interval: GroupSpendingLimitInterval = GroupSpendingLimitInterval.DAILY,
    val amount: String = "",
    val currency: String = "",
) : Parcelable
