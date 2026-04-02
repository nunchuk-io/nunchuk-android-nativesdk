package com.nunchuk.android.type;

// This class should be kept in java since jni can't access object class
public final class SignerTagHelper {

    private SignerTagHelper() {
    }

    public static SignerTag from(final int ordinal) {
        return CollectionUtil.fromOrdinal(SignerTag.values(),
                ordinal,
                SignerTag.INHERITANCE
        );
    }
}