package com.nunchuk.android.listener

import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.Job
import kotlinx.coroutines.flow.MutableSharedFlow
import kotlinx.coroutines.flow.asSharedFlow
import kotlinx.coroutines.launch

object GroupDeleteListener {
    private val appScope = CoroutineScope(Job() + Dispatchers.IO)
    private val _groupDeleteFlow = MutableSharedFlow<String>()
    val groupDeleteFlow = _groupDeleteFlow.asSharedFlow()

    @JvmStatic
    fun onGroupDelete(groupId: String) {
        appScope.launch {
            _groupDeleteFlow.emit(groupId)
        }
    }
}