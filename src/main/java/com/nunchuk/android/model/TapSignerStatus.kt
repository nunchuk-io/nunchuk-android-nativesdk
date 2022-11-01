package com.nunchuk.android.model

import android.os.Parcelable
import kotlinx.parcelize.Parcelize

@Parcelize
data class TapSignerStatus(
    override var isNeedSetup: Boolean = false,
    var isCreateSigner: Boolean = false,
    var backupKey: ByteArray? = null,
    var masterSignerId: String? = null,
    var version: String? = null,
    var birthHeight: Int = 0,
    override var ident: String? = null,
    override var authDelayInSecond: Int = 0
) : CardStatus(), Parcelable {

    override fun equals(other: Any?): Boolean {
        if (this === other) return true
        if (other !is TapSignerStatus) return false

        if (isNeedSetup != other.isNeedSetup) return false
        if (isCreateSigner != other.isCreateSigner) return false
        if (backupKey != null) {
            if (other.backupKey == null) return false
            if (!backupKey.contentEquals(other.backupKey)) return false
        } else if (other.backupKey != null) return false
        if (ident != other.ident) return false
        if (authDelayInSecond != other.authDelayInSecond) return false

        return true
    }

    override fun hashCode(): Int {
        var result = isNeedSetup.hashCode()
        result = 31 * result + isCreateSigner.hashCode()
        result = 31 * result + (backupKey?.contentHashCode() ?: 0)
        result = 31 * result + (ident?.hashCode() ?: 0)
        result = 31 * result + authDelayInSecond
        return result
    }
}
