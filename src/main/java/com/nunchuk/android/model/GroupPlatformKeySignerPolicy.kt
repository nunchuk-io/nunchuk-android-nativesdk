package com.nunchuk.android.model

import android.os.Parcelable
import kotlinx.parcelize.Parcelize

@Parcelize
data class GroupPlatformKeySignerPolicy(
    val masterFingerprint: String = "",
    val policy: GroupPlatformKeyPolicy = GroupPlatformKeyPolicy(),
) : Parcelable
