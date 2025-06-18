package com.nunchuk.android.model

data class MiniscriptTemplateResult(
    val template: String,
    val isValidTapscript: Boolean
) {
    // Default constructor for JNI
    constructor() : this("", false)
} 