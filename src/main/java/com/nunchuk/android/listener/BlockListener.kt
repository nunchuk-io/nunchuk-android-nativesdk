package com.nunchuk.android.listener

import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.Job
import kotlinx.coroutines.flow.MutableSharedFlow
import kotlinx.coroutines.flow.asSharedFlow
import kotlinx.coroutines.launch

object BlockListener {
    private val appScope = CoroutineScope(Job() + Dispatchers.Main.immediate)
    private val blockChainFlow = MutableSharedFlow<BtcBlockInfo>()

    @JvmStatic
    fun onBlockUpdate(height: Int, hexHeader: String) {
        appScope.launch {
            blockChainFlow.emit(BtcBlockInfo(height, hexHeader))
        }
    }

    fun getBlockChainFlow() = blockChainFlow.asSharedFlow()
}

data class BtcBlockInfo(val height: Int, val hexHeader: String)