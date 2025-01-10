package com.nunchuk.android.model

import android.os.Parcelable
import kotlinx.parcelize.Parcelize

@Parcelize
data class FreeGroupMessage(
    var id: String,
    var walletId: String,
    var sender: String,
    var content: String,
    var timestamp: Long,
) : Parcelable