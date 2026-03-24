package com.nunchuk.android.model

data class GroupPlatformKeyPolicies(
    val global: GroupPlatformKeyPolicy? = null,
    val signers: List<GroupPlatformKeySignerPolicy> = emptyList(),
)
