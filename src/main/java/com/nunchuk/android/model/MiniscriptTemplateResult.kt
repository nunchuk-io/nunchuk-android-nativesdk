package com.nunchuk.android.model

data class MiniscriptTemplateResult(
    val template: String,
    val isValidTapscript: Boolean,
    val isValidPolicy: Boolean,
    val isValidMiniscriptTemplate: Boolean
) {
    // Default constructor for JNI
    constructor() : this("", false, false, false)
} 