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

## JNI coding rules

Follow these rules when writing or modifying C++ JNI code. Violating them causes memory leaks or crashes on Android.

### String handling — always use `StringWrapper`

**Never** call `env->GetStringUTFChars()` directly. Use `StringWrapper(env, jstring)` from `string-wrapper.h` instead. It calls `ReleaseStringUTFChars` automatically in its destructor.

```cpp
// BAD — leaks the native string pointer
settings.set_name(env->GetStringUTFChars(name, JNI_FALSE));

// GOOD — RAII, automatically released
settings.set_name(StringWrapper(env, name));
```

### Returning strings to Java — always delete the local ref

When creating a `jstring` with `NewStringUTF` to pass to a Java setter, store it, use it, then delete it. In `deserializer.cpp`, use the `callSetString` helper:

```cpp
// BAD — leaks the jstring local reference
env->CallVoidMethod(instance, setNameMethod, env->NewStringUTF(name.c_str()));

// GOOD — helper creates, passes, and deletes the jstring
callSetString(env, instance, setNameMethod, name.c_str());
```

When passing `NewStringUTF` to a constructor (`NewObject`), store in a variable and `DeleteLocalRef` after:

```cpp
jstring jName = env->NewStringUTF(name.c_str());
jobject instance = env->NewObject(clazz, constructor, jName);
env->DeleteLocalRef(jName);
```

### Catch blocks — never return `ExceptionOccurred()`

`env->ExceptionOccurred()` returns `jthrowable`, not `jobject`. Return `nullptr` instead:

```cpp
// BAD — type mismatch, undefined behavior
} catch (BaseException &e) {
    Deserializer::convert2JException(env, e);
    return env->ExceptionOccurred();
}

// GOOD
} catch (BaseException &e) {
    Deserializer::convert2JException(env, e);
    return nullptr;
}
```

### Lambda callbacks — clean up local refs

Listener lambdas (block, transaction, group, etc.) run repeatedly. Any `NewStringUTF` or `NewObject` inside them must be stored and deleted:

```cpp
jstring jId = g_env->NewStringUTF(id.c_str());
g_env->CallStaticVoidMethod(clazz, method, jId);
g_env->DeleteLocalRef(jId);
```

### FindClass — cache as static global refs for hot paths

`env->FindClass()` returns a local ref. For functions called frequently (e.g., `convert2JBoolean`, `convert2JInt`), cache the result as a `static` global ref:

```cpp
static auto clazz = static_cast<jclass>(env->NewGlobalRef(env->FindClass("java/lang/Boolean")));
```

### Heap allocations — prefer RAII containers

Use `std::vector` instead of `new[]`/`delete[]`:

```cpp
// BAD — leak if exception is thrown before delete[]
auto* buf = new unsigned char[len];

// GOOD
std::vector<unsigned char> buf(len);
```
