package com.nunchuk.android.model

data class CoinsGroup(
    val coins: List<UnspentOutput>,
    val start: Long,
    val end: Long,
)