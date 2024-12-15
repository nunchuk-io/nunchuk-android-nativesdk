package com.nunchuk.android.model

import android.os.Parcelable
import com.nunchuk.android.type.TransactionStatus
import kotlinx.parcelize.Parcelize

@Parcelize
data class KeySetStatus(
    var status: TransactionStatus,
    var signerStatus: Map<String, Boolean> = emptyMap()
) : Parcelable