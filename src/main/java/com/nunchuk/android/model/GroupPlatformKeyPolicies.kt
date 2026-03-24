package com.nunchuk.android.model

import android.os.Parcelable
import kotlinx.parcelize.Parcelize

@Parcelize
data class GroupPlatformKeyPolicies(
    val global: GroupPlatformKeyPolicy? = null,
    val signers: List<GroupPlatformKeySignerPolicy> = emptyList(),
) : Parcelable
