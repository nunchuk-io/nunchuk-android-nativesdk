package com.nunchuk.android.model

interface SendEventExecutor {
    fun execute(type: String, content: String): String
}