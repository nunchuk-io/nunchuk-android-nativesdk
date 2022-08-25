package com.nunchuk.android.model

import kotlinx.coroutines.flow.MutableSharedFlow

object BlockListener {
    private val blockChainFlow = MutableSharedFlow<BtcBlockInfo>()

    @JvmStatic
    fun onBlockUpdate(height: Int, hexHeader: String) {
        blockChainFlow.tryEmit(BtcBlockInfo(height, hexHeader))
    }
}

data class BtcBlockInfo(val height: Int, val hexHeader: String)