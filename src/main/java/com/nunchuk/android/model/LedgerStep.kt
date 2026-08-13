package com.nunchuk.android.model

import androidx.annotation.Keep
import com.nunchuk.android.type.LedgerStepType
import com.nunchuk.android.type.LedgerUserInteraction

// Constructed by JNI (see ledger-jni.cpp). Mirrors nunchuk::ledger::LedgerStep.
//
// `type` / `interaction` are stored as enum ordinals to keep the JNI layer simple;
// use `stepType` / `userInteraction` to read them as enums.
@Keep
data class LedgerStep(
    var type: Int = LedgerStepType.FAILED.ordinal,
    var interaction: Int = LedgerUserInteraction.NONE.ordinal,
    var writes: List<ByteArray> = emptyList(),
    var error: LedgerError? = null,
) {
    val stepType: LedgerStepType
        get() = LedgerStepType.entries.getOrElse(type) { LedgerStepType.FAILED }

    val userInteraction: LedgerUserInteraction
        get() = LedgerUserInteraction.entries.getOrElse(interaction) { LedgerUserInteraction.NONE }
}
