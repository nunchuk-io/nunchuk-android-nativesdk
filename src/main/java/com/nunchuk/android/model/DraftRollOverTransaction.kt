package com.nunchuk.android.model

import android.os.Parcelable
import kotlinx.parcelize.Parcelize

@Parcelize
data class DraftRollOverTransaction(
    var transaction: Transaction = Transaction(),
    var tagIds: Set<Int> = emptySet(),
    var collectionIds: Set<Int> = emptySet(),
) : Parcelable