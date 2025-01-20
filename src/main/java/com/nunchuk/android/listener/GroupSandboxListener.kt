package com.nunchuk.android.listener

import com.nunchuk.android.model.GroupSandbox
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.Job
import kotlinx.coroutines.flow.MutableSharedFlow
import kotlinx.coroutines.launch

object GroupSandboxListener {
    private val appScope = CoroutineScope(Job() + Dispatchers.IO)
    private val groupFlow = MutableSharedFlow<GroupSandbox>()

    @JvmStatic
    fun onGroupUpdate(group: GroupSandbox) {
        appScope.launch {
            groupFlow.emit(group)
        }
    }

    fun getGroupFlow() = groupFlow
}