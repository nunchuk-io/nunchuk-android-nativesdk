package com.nunchuk.android.type

import androidx.annotation.Keep

// Ordinals must match nunchuk::bitbox::UserInteraction in libnunchuk (types.hpp).
//
// Polling can return the same interaction many times; only surface a non-NONE
// interaction to the user when it changes.
@Keep
enum class BitBoxUserInteraction {
    NONE,
    UNLOCK_DEVICE,
    CONFIRM_PAIRING,
    VERIFY_ADDRESS,
    REGISTER_WALLET,
    SIGN_MESSAGE,
    SIGN_TRANSACTION,
    CONFIRM_DEVICE_NAME,
    SET_DEVICE_PASSWORD,
    SHOW_RECOVERY_WORDS,
    INSERT_SD_CARD,
    CREATE_BACKUP,
    RESTORE_FROM_RECOVERY_WORDS,
    RESTORE_FROM_BACKUP,
    CONFIRM_FIRMWARE_UPGRADE,
    CHANGE_DEVICE_PASSWORD,
    TOGGLE_MNEMONIC_PASSPHRASE,
    CHECK_BACKUP,
    FACTORY_RESET,
}
