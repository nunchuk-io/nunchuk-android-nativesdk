package com.nunchuk.android.model

data class GroupPlatformKeyPolicyUpdateRequirement(
    val success: Boolean = false,
    val delayApplyInSeconds: Int = 0,
    val requiresDummyTransaction: Boolean = false,
    val dummyTransaction: GroupDummyTransaction? = null,
)
