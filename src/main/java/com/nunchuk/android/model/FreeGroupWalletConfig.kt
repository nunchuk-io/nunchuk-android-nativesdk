package com.nunchuk.android.model

import android.os.Parcelable
import kotlinx.parcelize.Parcelize

@Parcelize
data class FreeGroupWalletConfig(
    val chatRetentionDays: Int,
    val platformKey: GroupPlatformKey? = null,
    val platformKeyFingerprint: String = "",
) : Parcelable