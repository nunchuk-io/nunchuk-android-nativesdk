package com.nunchuk.android.type;

// This class should be kept in java since jni can't access object class
public final class AddressTypeHelper {

    private AddressTypeHelper() {
    }

    public static AddressType from(final int ordinal) {
        return CollectionUtil.fromOrdinal(AddressType.values(),
                ordinal,
                AddressType.NATIVE_SEGWIT
        );
    }

}