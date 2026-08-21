package com.nunchuk.android.type

import androidx.annotation.Keep

/**
 * Mirrors nunchuk::bitbox::BitBoxMnemonicLength in libnunchuk (types.hpp) — the
 * underlying values are the word counts themselves, not ordinals.
 *
 * [WORDS_12] requires device firmware 9.6 or newer.
 */
@Keep
enum class BitBoxMnemonicLength(val words: Int) {
    WORDS_12(12),
    WORDS_24(24),
}
