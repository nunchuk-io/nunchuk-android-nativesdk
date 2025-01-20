package com.nunchuk.android.listener

import com.nunchuk.android.model.GroupOnline
import com.nunchuk.android.model.GroupSandbox
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.Job
import kotlinx.coroutines.flow.MutableSharedFlow
import kotlinx.coroutines.launch

object GroupOnlineListener {
    private val appScope = CoroutineScope(Job() + Dispatchers.IO)
    private val groupFlow = MutableSharedFlow<GroupOnline>()

    @JvmStatic
    fun onGroupOnlineUpdate(groupId: String, online: Int) {
        appScope.launch {
            groupFlow.emit(GroupOnline(groupId, online))
        }
    }

    fun getGroupFlow() = groupFlow
}