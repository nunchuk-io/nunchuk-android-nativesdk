package com.nunchuk.android.model

import com.nunchuk.android.type.ConnectionStatusTypeHelper
import kotlinx.coroutines.flow.MutableStateFlow

object ConnectionStatusHelper {
    // Bitcoin (normal wallet) synchronizer status.
    var blockChainStatus = MutableStateFlow<SyncBlockchainStatus?>(null)

    // Liquid synchronizer status, tracked separately from bitcoin.
    var liquidBlockChainStatus = MutableStateFlow<SyncBlockchainStatus?>(null)

    @JvmStatic
    fun addListener(status: Int, percent: Int, liquid: Boolean) {
        val connectionStatus = ConnectionStatusTypeHelper.from(status)
        val syncStatus = SyncBlockchainStatus(connectionStatus, percent, liquid)
        if (liquid) {
            liquidBlockChainStatus.value = syncStatus
        } else {
            blockChainStatus.value = syncStatus
        }
    }
}
