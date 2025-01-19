package com.nunchuk.android.listener

import kotlinx.coroutines.flow.MutableSharedFlow
import kotlinx.coroutines.flow.asSharedFlow

object GroupDeleteListener {
    private val _groupDeleteFlow = MutableSharedFlow<String>()
    val groupDeleteFlow = _groupDeleteFlow.asSharedFlow()

    @JvmStatic
    fun onGroupDelete(groupId: String) {
        _groupDeleteFlow.tryEmit(groupId)
    }
}