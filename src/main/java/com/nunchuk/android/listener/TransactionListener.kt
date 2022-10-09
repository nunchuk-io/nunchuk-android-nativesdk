package com.nunchuk.android.listener

import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.Job
import kotlinx.coroutines.flow.MutableSharedFlow
import kotlinx.coroutines.flow.asSharedFlow
import kotlinx.coroutines.launch

object TransactionListener {
    private val appScope = CoroutineScope(Job() + Dispatchers.IO)
    private val internalTransactionUpdateFlow = MutableSharedFlow<TransactionUpdate>()
    val transactionUpdateFlow = internalTransactionUpdateFlow.asSharedFlow()

    @JvmStatic
    fun onTransactionUpdate(txId: String, walletId: String) {
        appScope.launch {
            internalTransactionUpdateFlow.emit(TransactionUpdate(txId, walletId))
        }
    }
}

data class TransactionUpdate(val txId: String, val walletId: String)