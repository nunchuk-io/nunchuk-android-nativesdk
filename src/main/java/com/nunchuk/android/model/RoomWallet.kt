package com.nunchuk.android.model

data class RoomWallet(
    var roomId:String = "",
    var walletId: String = "",
    var initEventId: String = "",
    var joinEventIds: List<String> = emptyList(),
    var leaveEventIds: List<String> = emptyList(),
    var finalizeEventId: String = "",
    var cancelEventId: String = "",
    var readyEventId: String = "",
    var deleteEventId:String = "",
    var jsonContent: String = "",
)