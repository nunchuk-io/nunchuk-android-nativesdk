package com.nunchuk.android.model

interface SendEventExecutor {
    fun execute(roomId: String, type: String, content: String): String
}