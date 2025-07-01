package com.nunchuk.android.model

data class ScriptNodeResult(
    val scriptNode: ScriptNode,
    val keyPath: String
) {
    // Default constructor for JNI
    constructor() : this(ScriptNode(emptyList(), "", emptyList(), emptyList(), 0, byteArrayOf()), "")
} 