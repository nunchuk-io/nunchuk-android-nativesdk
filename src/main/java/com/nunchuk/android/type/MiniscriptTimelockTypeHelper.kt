package com.nunchuk.android.type;

public class MiniscriptTimelockTypeHelper {
    public static MiniscriptTimelockType from(int ordinal) {
        switch (ordinal) {
            case 0:
                return MiniscriptTimelockType.ABSOLUTE;
            case 1:
                return MiniscriptTimelockType.RELATIVE;
            default:
                return MiniscriptTimelockType.ABSOLUTE;
        }
    }
} 