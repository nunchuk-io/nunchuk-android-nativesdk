package com.nunchuk.android.model

import android.os.Parcelable
import kotlinx.parcelize.Parcelize

@Parcelize
data class GroupPlatformKey(
    val policies: GroupPlatformKeyPolicies = GroupPlatformKeyPolicies(),
) : Parcelable
