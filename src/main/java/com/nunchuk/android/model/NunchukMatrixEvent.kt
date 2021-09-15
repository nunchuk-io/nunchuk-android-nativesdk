package com.nunchuk.android.model

data class NunchukMatrixEvent(
    var type: String = "",
    var content: String = "",
    var eventId: String = "",
    var roomId: String = "",
    var sender: String = "",
    var time: Long = 0
)
