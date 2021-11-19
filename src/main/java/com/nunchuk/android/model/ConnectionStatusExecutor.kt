package com.nunchuk.android.model

import com.nunchuk.android.type.ConnectionStatus

interface ConnectionStatusExecutor {
    fun execute(connectionStatus: ConnectionStatus, percent: Int)
}