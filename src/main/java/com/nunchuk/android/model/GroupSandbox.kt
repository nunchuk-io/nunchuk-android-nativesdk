package com.nunchuk.android.model

import android.os.Parcelable
import com.nunchuk.android.type.AddressType
import kotlinx.parcelize.Parcelize

@Parcelize
data class GroupSandbox(
    val id : String,
    val name : String,
    val url : String,
    val m: Int,
    val n: Int,
    val addressType: AddressType,
    val signers: List<SingleSigner>,
    val finalized: Boolean,
    val walletId: String,
    val occupiedSlots: List<OccupiedSlot?>,
) : Parcelable