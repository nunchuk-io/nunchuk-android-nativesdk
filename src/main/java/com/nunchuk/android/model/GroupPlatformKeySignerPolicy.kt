package com.nunchuk.android.model

data class GroupPlatformKeySignerPolicy(
    val masterFingerprint: String = "",
    val policy: GroupPlatformKeyPolicy = GroupPlatformKeyPolicy(),
)
