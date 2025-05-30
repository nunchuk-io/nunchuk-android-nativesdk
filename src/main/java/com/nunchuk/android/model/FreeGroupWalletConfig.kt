package com.nunchuk.android.model

import android.os.Parcelable
import kotlinx.parcelize.Parcelize

@Parcelize
data class FreeGroupWalletConfig(
    var chatRetentionDays: Int,
) : Parcelable