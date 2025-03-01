package com.nunchuk.android.listener

import com.nunchuk.android.model.ReplacingGroupWallet
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.Job
import kotlinx.coroutines.flow.MutableSharedFlow
import kotlinx.coroutines.flow.asSharedFlow
import kotlinx.coroutines.launch

object GroupReplaceListener {
    private val appScope = CoroutineScope(Job() + Dispatchers.IO)
    private val _groupReplaceFlow = MutableSharedFlow<ReplacingGroupWallet>()
    val groupReplaceFlow = _groupReplaceFlow.asSharedFlow()

    @JvmStatic
    fun onGroupReplace(walletId: String, groupId: String) {
        appScope.launch {
            _groupReplaceFlow.emit(ReplacingGroupWallet(walletId, groupId))
        }
    }
}