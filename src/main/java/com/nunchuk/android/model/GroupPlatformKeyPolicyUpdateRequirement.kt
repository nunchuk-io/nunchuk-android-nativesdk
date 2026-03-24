package com.nunchuk.android.model

data class GroupPlatformKeyPolicyUpdateRequirement(
    val success: Boolean = false,
    val requiresDummyTransaction: Boolean = false,
    val dummyTransaction: GroupDummyTransaction? = null,
)
