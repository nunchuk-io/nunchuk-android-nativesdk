package com.nunchuk.android.model

import android.os.Parcelable
import kotlinx.parcelize.Parcelize

@Parcelize
data class FreeGroupWalletConfig(
    var total: Int,
    var remain: Int,
    var maxKey: Int,
) : Parcelable