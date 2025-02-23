package com.nunchuk.android.model

import com.nunchuk.android.type.ConnectionStatusTypeHelper
import kotlinx.coroutines.flow.MutableStateFlow

object ConnectionStatusHelper {
    var blockChainStatus = MutableStateFlow<SyncBlockchainStatus?>(null)

    @JvmStatic
    fun addListener(status: Int, percent: Int) {
        val connectionStatus = ConnectionStatusTypeHelper.from(status)
        blockChainStatus.value = SyncBlockchainStatus(connectionStatus, percent)
    }
}
