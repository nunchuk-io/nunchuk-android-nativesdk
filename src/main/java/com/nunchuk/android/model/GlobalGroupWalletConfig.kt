package com.nunchuk.android.model

import android.os.Parcelable
import kotlinx.parcelize.Parcelize

@Parcelize
data class GlobalGroupWalletConfig(
    var total: Int,
    var remain: Int,
    var maxKey: Int,
    var retentionDaysOptions: Set<Int> = emptySet(),
) : Parcelable