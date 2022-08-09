package com.nunchuk.android.model

import android.os.Parcelable
import kotlinx.parcelize.Parcelize

@Parcelize
data class SatsCardStatus(
    override var isNeedSetup: Boolean = false,
    var activeSlotIndex: Int = -1,
    var numberOfSlot: Int = -1,
    var isUsedUp: Boolean = false,
    override var ident: String? = null,
    override var authDelayInSecond: Int = 0,
    var slots: List<SatsCardSlot> = mutableListOf()
) : CardStatus(), Parcelable