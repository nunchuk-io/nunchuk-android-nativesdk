package com.nunchuk.android.model

data class NunchukMatrixEvent(
    var type: String = "",
    var content: String = "",
    var eventId: String = "",
    var roomId: String = "",
    var sender: String = "",
    var time: Long = 0
)

data class RoomWallet(
    var walletId: String = "",
    var initEventId: String = "",
    var joinEventIds: List<String> = emptyList(),
    var leaveEventIds: List<String> = emptyList(),
    var finalizeEventId: String = "",
    var cancelEventId: String = "",
    var readyEventId: String = "",
    var pinData: String = "",
)

data class RoomTransaction(
    var txId: String = "",
    var walletId: String = "",
    var initEventId: String = "",
    var signEventIds: List<String> = emptyList(),
    var rejectEventIds: List<String> = emptyList(),
    var broadcastEventId: String = "",
    var cancelEventId: String = "",
    var readyEventId: String = ""
)
