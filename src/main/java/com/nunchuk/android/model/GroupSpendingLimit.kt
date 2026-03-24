package com.nunchuk.android.model

import com.nunchuk.android.type.GroupSpendingLimitInterval

data class GroupSpendingLimit(
    val interval: GroupSpendingLimitInterval = GroupSpendingLimitInterval.DAILY,
    val amount: String = "",
    val currency: String = "",
)
