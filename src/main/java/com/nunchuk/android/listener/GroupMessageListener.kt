package com.nunchuk.android.listener

import com.nunchuk.android.model.FreeGroupMessage
import com.nunchuk.android.model.GroupSandbox
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.Job
import kotlinx.coroutines.flow.MutableSharedFlow
import kotlinx.coroutines.launch

object GroupMessageListener {
    private val appScope = CoroutineScope(Job() + Dispatchers.Main.immediate)
    private val messageFlow = MutableSharedFlow<FreeGroupMessage>()

    @JvmStatic
    fun onGroupMessageUpdate(group: FreeGroupMessage) {
        appScope.launch {
            messageFlow.emit(group)
        }
    }

    fun getMessageFlow() = messageFlow
}