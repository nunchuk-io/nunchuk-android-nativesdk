package com.nunchuk.android.model

import android.os.Parcelable
import kotlinx.parcelize.Parcelize

@Parcelize
data class ScriptNode(
    val type: String,
    val keys: List<String>,
    val subs: List<ScriptNode>,
    val k: Int
): Parcelable