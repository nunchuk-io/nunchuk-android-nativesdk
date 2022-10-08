package com.nunchuk.android.listener

import com.nunchuk.android.type.TransactionStatus
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
    fun onTransactionUpdate(txId: String, status: TransactionStatus, walletId: String) {
        appScope.launch {
            internalTransactionUpdateFlow.emit(TransactionUpdate(txId, status, walletId))
        }
    }
}

data class TransactionUpdate(val txId: String, val status: TransactionStatus, val walletId: String)