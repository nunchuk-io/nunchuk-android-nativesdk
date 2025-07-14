package com.nunchuk.android.model

import kotlinx.parcelize.RawValue

data class ScriptNodeResult(
    val scriptNode: ScriptNode,
    val keyPath: String
) {
    // Default constructor for JNI
    constructor() : this(ScriptNode(emptyList(), "", emptyList(), emptyList(), 0, byteArrayOf(), null as @RawValue TimeLock?), "")
} 