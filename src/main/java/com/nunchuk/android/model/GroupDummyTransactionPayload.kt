package com.nunchuk.android.model

data class GroupDummyTransactionPayload(
    val oldPolicies: GroupPlatformKeyPolicies = GroupPlatformKeyPolicies(),
    val newPolicies: GroupPlatformKeyPolicies = GroupPlatformKeyPolicies(),
)
