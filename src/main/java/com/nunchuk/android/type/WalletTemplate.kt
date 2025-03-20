package com.nunchuk.android.type

enum class WalletTemplate {
    DEFAULT,
    DISABLE_KEY_PATH;

    companion object {
        @JvmStatic
        fun from(value: Int): WalletTemplate {
            return entries[value]
        }
    }
}