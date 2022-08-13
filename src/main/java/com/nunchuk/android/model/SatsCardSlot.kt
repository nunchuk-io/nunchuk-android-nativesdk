package com.nunchuk.android.model

import android.os.Parcelable
import com.nunchuk.android.type.SatsCardSlotStatus
import kotlinx.parcelize.Parcelize

@Parcelize
data class SatsCardSlot(
    var index: Int = 0,
    var status: SatsCardSlotStatus = SatsCardSlotStatus.UNUSED,
    var address: String? = null,
    var balance: Amount = Amount(),
    var isConfirmed: Boolean = false,
    var outputs: List<UnspentOutput> = emptyList(),
    var privateKey: ByteArray = ByteArray(0),
    var publicKey: ByteArray = ByteArray(0),
    var chainCode: ByteArray = ByteArray(0),
    var masterPrivateKey: ByteArray = ByteArray(0),
) : Parcelable {

    override fun equals(other: Any?): Boolean {
        if (this === other) return true
        if (other !is SatsCardSlot) return false

        if (index != other.index) return false
        if (status != other.status) return false
        if (address != other.address) return false
        if (balance != other.balance) return false
        if (isConfirmed != other.isConfirmed) return false
        if (outputs != other.outputs) return false
        if (!privateKey.contentEquals(other.privateKey)) return false
        if (!publicKey.contentEquals(other.publicKey)) return false
        if (!chainCode.contentEquals(other.chainCode)) return false
        if (!masterPrivateKey.contentEquals(other.masterPrivateKey)) return false

        return true
    }

    override fun hashCode(): Int {
        var result = index
        result = 31 * result + status.hashCode()
        result = 31 * result + (address?.hashCode() ?: 0)
        result = 31 * result + balance.hashCode()
        result = 31 * result + isConfirmed.hashCode()
        result = 31 * result + outputs.hashCode()
        result = 31 * result + privateKey.contentHashCode()
        result = 31 * result + publicKey.contentHashCode()
        result = 31 * result + chainCode.contentHashCode()
        result = 31 * result + masterPrivateKey.contentHashCode()
        return result
    }
}