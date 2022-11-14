package com.nunchuk.android.type

import androidx.annotation.Keep

@Keep
enum class TransactionStatus {
    PENDING_SIGNATURES,
    READY_TO_BROADCAST,
    NETWORK_REJECTED,
    PENDING_CONFIRMATION,
    REPLACED,
    CONFIRMED,
}