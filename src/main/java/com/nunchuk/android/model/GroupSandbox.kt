package com.nunchuk.android.model

import com.nunchuk.android.type.AddressType

data class GroupSandbox(
    val id : String,
    val name : String,
    val url : String,
    val m: Int,
    val n: Int,
    val addressType: AddressType,
    val signers: List<SingleSigner>,
    val finalized: Boolean,
    val walletId: String
)