package com.nunchuk.android.model

import com.nunchuk.android.type.GroupDummyTransactionStatus
import com.nunchuk.android.type.GroupDummyTransactionType

data class GroupDummyTransaction(
    val id: String = "",
    val walletId: String = "",
    val type: GroupDummyTransactionType = GroupDummyTransactionType.UNKNOWN,
    val status: GroupDummyTransactionStatus = GroupDummyTransactionStatus.PENDING_SIGNATURES,
    val payload: GroupDummyTransactionPlatformKeyPolicyData? = null,
    val requiredSignatures: Int = 0,
    val pendingSignatures: Int = 0,
    val requestBody: String = "",
    val signatures: List<GroupDummyTransactionSignature> = emptyList(),
    val createdAt: Long = 0,
)
