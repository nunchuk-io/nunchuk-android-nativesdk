package com.nunchuk.android.model

data class RoomTransaction(
    var roomId: String = "",
    var walletId: String = "",
    var txId: String = "",
    var initEventId: String = "",
    var signEventIds: List<String> = emptyList(),
    var rejectEventIds: List<String> = emptyList(),
    var broadcastEventId: String = "",
    var cancelEventId: String = "",
    var readyEventId: String = ""
)