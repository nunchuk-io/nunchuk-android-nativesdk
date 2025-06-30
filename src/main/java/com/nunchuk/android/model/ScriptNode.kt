package com.nunchuk.android.model

import android.os.Parcelable
import kotlinx.parcelize.Parcelize

@Parcelize
data class ScriptNode(
    val type: String,
    val keys: List<String>,
    val subs: List<ScriptNode>,
    val k: Int,
    val data: ByteArray
): Parcelable {
    override fun equals(other: Any?): Boolean {
        if (this === other) return true
        if (javaClass != other?.javaClass) return false

        other as ScriptNode

        if (k != other.k) return false
        if (type != other.type) return false
        if (keys != other.keys) return false
        if (subs != other.subs) return false
        if (!data.contentEquals(other.data)) return false

        return true
    }

    override fun hashCode(): Int {
        var result = k
        result = 31 * result + type.hashCode()
        result = 31 * result + keys.hashCode()
        result = 31 * result + subs.hashCode()
        result = 31 * result + data.contentHashCode()
        return result
    }
}