package com.nunchuk.android.model

import com.nunchuk.android.type.GroupWalletAlertType

data class GroupWalletAlert(
    val id: String = "",
    val type: GroupWalletAlertType = GroupWalletAlertType.UNKNOWN,
    val viewable: Boolean = false,
    val title: String = "",
    val body: String = "",
    val payload: GroupWalletAlertPayload? = null,
    val createdAt: Long = 0,
)
