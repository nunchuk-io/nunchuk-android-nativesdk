package com.nunchuk.android.type;

// This class should be kept in java since jni can't access object class
public final class SignerTypeHelper {

    private SignerTypeHelper() {
    }

    public static SignerType from(final int ordinal) {
        return CollectionUtil.fromOrdinal(SignerType.values(),
                ordinal,
                SignerType.AIRGAP
        );
    }

}