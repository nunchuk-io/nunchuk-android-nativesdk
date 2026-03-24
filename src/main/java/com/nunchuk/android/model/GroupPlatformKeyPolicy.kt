package com.nunchuk.android.model

import android.os.Parcelable
import kotlinx.parcelize.Parcelize

@Parcelize
data class GroupPlatformKeyPolicy(
    val autoBroadcastTransaction: Boolean = false,
    val signingDelaySeconds: Int = 0,
    val spendingLimit: GroupSpendingLimit? = null,
) : Parcelable
