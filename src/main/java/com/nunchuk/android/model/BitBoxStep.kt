package com.nunchuk.android.model

import androidx.annotation.Keep
import com.nunchuk.android.type.BitBoxStepType
import com.nunchuk.android.type.BitBoxUserInteraction

/**
 * Constructed by JNI (see bitbox-jni.cpp). Mirrors nunchuk::bitbox::BitBoxStep.
 *
 * `type` / `interaction` are stored as enum ordinals to keep the JNI layer simple;
 * use [stepType] / [userInteraction] to read them as enums.
 *
 * Native `std::optional` fields are flattened for JNI: [pairingCode] and [error] are
 * null when absent, and [progress] uses [NO_PROGRESS] as its absent sentinel — read it
 * through [progressOrNull].
 */
@Keep
data class BitBoxStep(
    var type: Int = BitBoxStepType.FAILED.ordinal,
    var interaction: Int = BitBoxUserInteraction.NONE.ordinal,
    var writes: List<ByteArray> = emptyList(),
    var retryAfterMs: Int = 0,
    var pairingCode: String? = null,
    var error: BitBoxError? = null,
    var progress: Double = NO_PROGRESS,
) {
    val stepType: BitBoxStepType
        get() = BitBoxStepType.entries.getOrElse(type) { BitBoxStepType.FAILED }

    val userInteraction: BitBoxUserInteraction
        get() = BitBoxUserInteraction.entries.getOrElse(interaction) { BitBoxUserInteraction.NONE }

    val progressOrNull: Double?
        get() = progress.takeIf { it >= 0.0 }

    /** COMPLETE, FAILED and REBOOT end an operation; anything else keeps driving it. */
    val isTerminal: Boolean
        get() = stepType == BitBoxStepType.COMPLETE ||
            stepType == BitBoxStepType.FAILED ||
            stepType == BitBoxStepType.REBOOT

    companion object {
        const val NO_PROGRESS = -1.0
    }
}
