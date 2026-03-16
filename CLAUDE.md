# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is **Nunchuk Android Native SDK** — a JNI wrapper around [libnunchuk](https://github.com/nunchuk-io/libnunchuk) (a C++ Bitcoin multisig library). It produces an Android AAR library (`io.nunchuk.android:nativesdk`) consumed by the Nunchuk Android app.

## Build Commands

```bash
# Prerequisites: NDK 27.2.12479018, CMake 3.22.1, Android SDK
export ANDROID_SDK=/Users/${USER}/Library/Android/sdk
export ANDROID_NDK_HOME=$ANDROID_SDK/ndk/27.2.12479018

# Build (arm64 + armeabi-v7a)
./gradlew clean assembleArmRelease --stacktrace

# Build single arch (faster for dev)
./gradlew assembleArm64_v8aRelease
./gradlew assembleArmeabi_v7aRelease

# Publish to local Maven
./gradlew publish
```

The `arch.processor` in `local.properties` controls which flavor is published (default: `arm`).

Product flavors: `arm` (both ARM ABIs), `arm64_v8a`, `armeabi_v7a`, `x86_64`, `universal`.

## Architecture

### Two-layer JNI bridge

1. **Kotlin layer** (`src/main/java/com/nunchuk/android/`):
   - `nativelib/LibNunchukAndroid.kt` — declares all JNI `external` functions (private API)
   - `nativelib/NunchukNativeSdk.kt` — public-facing SDK class that wraps `LibNunchukAndroid`, converting Kotlin model types to/from JNI primitives
   - `model/` — Kotlin data classes (Wallet, Transaction, SingleSigner, MasterSigner, etc.)
   - `type/` — enums (AddressType, Chain, SignerType, TransactionStatus, etc.)
   - `exception/` — exception types thrown across JNI boundary
   - `callbacks/` and `listener/` — callback interfaces for async operations (sync, block, NFC)

2. **C++ JNI layer** (`src/main/native/`):
   - `*-jni.cpp` files — JNI function implementations organized by domain (wallet, signer, transaction, room, sync, tap-protocol, satscard, group-wallet, util)
   - `serializer.cpp/.h` — converts Java objects to C++ libnunchuk types
   - `deserializer.cpp/.h` — converts C++ libnunchuk types to Java objects
   - `nunchukprovider.cpp/.h` — singleton holding `Nunchuk` and `NunchukMatrix` instances
   - `initializer.cpp/.h` — SDK initialization logic
   - `nfc.cpp/.h` — NFC/tap-protocol communication helpers
   - `CMakeLists.txt` — links against `libnunchuk` (added as subdirectory)

### Key pattern: adding a new native API

1. Add the `external fun` declaration in `LibNunchukAndroid.kt`
2. Add the public wrapper in `NunchukNativeSdk.kt`
3. Implement the JNI function in the appropriate `*-jni.cpp` file
4. Use `Serializer` to convert Java→C++ args, call libnunchuk, use `Deserializer` to convert C++→Java return values
5. Add any new model/type Kotlin classes if needed

### Important notes

- The `src/main/native/libnunchuk/` submodule is an external C++ dependency — do not modify it directly in this repo.
- JNI method names must exactly match the package path: `Java_com_nunchuk_android_nativelib_LibNunchukAndroid_<methodName>`
- C++ exceptions are caught and converted to Java exceptions via `Deserializer::convert2JException`
- The native library is loaded as `"nunchuk-android"` (defined in `LibNunchukAndroid.kt`)
