package com.nunchuk.android.nfc;

import android.nfc.tech.IsoDep;

import java.io.IOException;

public class IsoDepCaller {
    public static byte[] transceive(IsoDep isoDep, byte[] data) throws IOException {
        return isoDep.transceive(data);
    }
}
