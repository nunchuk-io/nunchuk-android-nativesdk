package com.nunchuk.android.model

import com.nunchuk.android.type.GroupTransactionStatus

data class GroupTransactionState(
    val status: GroupTransactionStatus = GroupTransactionStatus.UNKNOWN,
    val message: String = "",
    val cosignAt: Long = 0,
)
