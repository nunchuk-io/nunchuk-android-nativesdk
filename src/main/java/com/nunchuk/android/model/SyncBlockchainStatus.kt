package com.nunchuk.android.model

import com.nunchuk.android.type.ConnectionStatus

data class SyncBlockchainStatus(
    val status: ConnectionStatus,
    val percent: Int
)