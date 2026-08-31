package com.nunchuk.android.type

import androidx.annotation.Keep

// Ordinals must match nunchuk::bitbox::BitBoxProduct in libnunchuk (types.hpp).
@Keep
enum class BitBoxProduct {
    UNKNOWN,
    NOVA_MULTI,
    NOVA_BITCOIN_ONLY,
    BITBOX02_MULTI,
    BITBOX02_BITCOIN_ONLY,
    ;

    /** Nova is the only model that speaks BLE; the rest are USB-only. */
    val isNova: Boolean get() = this == NOVA_MULTI || this == NOVA_BITCOIN_ONLY

    val isBitcoinOnly: Boolean
        get() = this == NOVA_BITCOIN_ONLY || this == BITBOX02_BITCOIN_ONLY
}
