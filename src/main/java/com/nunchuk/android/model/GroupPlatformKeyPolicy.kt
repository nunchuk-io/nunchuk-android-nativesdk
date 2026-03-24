package com.nunchuk.android.model

data class GroupPlatformKeyPolicy(
    val autoBroadcastTransaction: Boolean = false,
    val signingDelaySeconds: Int = 0,
    val spendingLimit: GroupSpendingLimit? = null,
)
