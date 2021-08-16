package com.nunchuk.android.model

interface SendEventExecutor {
    fun invoke(type: String, content: String): String
}