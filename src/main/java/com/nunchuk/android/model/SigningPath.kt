package com.nunchuk.android.model

import android.os.Parcelable
import kotlinx.parcelize.Parcelize

/**
 * Represents a signing path, which is a list of ScriptNodeId (each ScriptNodeId is a list of Int)
 */
@Parcelize
data class SigningPath(
    val path: List<List<Int>>
) : Parcelable 