package com.nunchuk.android.model

import android.os.Parcelable
import com.nunchuk.android.type.Chain
import kotlinx.parcelize.Parcelize

@Parcelize
data class PrimaryKey(
    var name: String = "", var masterFingerprint: String = "",
    var account: String = "", var address: String = "", var chain: Chain? = null
) : Parcelable