package com.nunchuk.android.model

import android.os.Parcelable
import kotlinx.parcelize.IgnoredOnParcel
import kotlinx.parcelize.Parcelize
import kotlinx.parcelize.RawValue

@Parcelize
data class ScriptNode(
    val id: List<Int>,
    val type: String,
    val keys: List<String>,
    val subs: List<ScriptNode>,
    val k: Int,
    val data: ByteArray,
    val timeLock: @RawValue TimeLock?
) : Parcelable {
    @IgnoredOnParcel
    val idString: String = id.joinToString(separator = ",")

    override fun equals(other: Any?): Boolean {
        if (this === other) return true
        if (javaClass != other?.javaClass) return false

        other as ScriptNode

        if (id != other.id) return false
        if (k != other.k) return false
        if (type != other.type) return false
        if (keys != other.keys) return false
        if (subs != other.subs) return false
        if (!data.contentEquals(other.data)) return false
        if (timeLock != other.timeLock) return false

        return true
    }

    override fun hashCode(): Int {
        var result = id.hashCode()
        result = 31 * result + k
        result = 31 * result + type.hashCode()
        result = 31 * result + keys.hashCode()
        result = 31 * result + subs.hashCode()
        result = 31 * result + data.contentHashCode()
        result = 31 * result + (timeLock?.hashCode() ?: 0)
        return result
    }
}