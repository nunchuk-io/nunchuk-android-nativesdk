package com.nunchuk.android.model

data class GroupDummyTransactionPlatformKeyPolicyData(
    val oldPolicies: GroupPlatformKeyPolicies = GroupPlatformKeyPolicies(),
    val newPolicies: GroupPlatformKeyPolicies = GroupPlatformKeyPolicies(),
)
