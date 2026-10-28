package com.nunchuk.android.listener

import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.Job
import kotlinx.coroutines.flow.MutableSharedFlow
import kotlinx.coroutines.flow.asSharedFlow
import kotlinx.coroutines.launch

/**
 * Fired by libnunchuk when a transaction returned by the connected Electrum server
 * fails signature verification and is dropped.
 */
object InvalidTransactionListener {
    private val appScope = CoroutineScope(Job() + Dispatchers.IO)
    private val internalInvalidTransactionFlow = MutableSharedFlow<InvalidTransaction>()
    val invalidTransactionFlow = internalInvalidTransactionFlow.asSharedFlow()

    @JvmStatic
    fun onInvalidTransaction(txId: String, walletId: String) {
        appScope.launch {
            internalInvalidTransactionFlow.emit(InvalidTransaction(txId, walletId))
        }
    }
}

data class InvalidTransaction(val txId: String, val walletId: String)
