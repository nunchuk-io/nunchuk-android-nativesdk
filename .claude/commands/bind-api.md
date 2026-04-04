# Bind Native API

Create a new JNI binding for a libnunchuk C++ method. This creates the full binding chain: Kotlin public API -> Kotlin JNI external -> C++ JNI implementation, with serializer/deserializer updates if needed.

## Input

$ARGUMENTS - The C++ method name to bind (e.g. `Utils::ParseBtcUri`, `CreateWallet`, `GetWallet`)

## Steps

### 0. Pull latest libnunchuk submodule

Before doing anything else, pull the latest code for the current branch of the `libnunchuk` submodule so that `nunchuk.h` is up-to-date:

```bash
cd src/main/native/libnunchuk && git pull && cd -
```

This ensures the C++ method signatures you read are the latest available.

### 1. Look up the C++ method signature

- If the method name contains `Utils::`, look up the method in `src/main/native/libnunchuk/include/nunchuk.h` (under the `Utils` class) AND `src/main/native/libnunchuk/src/nunchukutils.cpp` for the full implementation details and parameter types.
- Otherwise, look up the method in `src/main/native/libnunchuk/include/nunchuk.h` (under the `Nunchuk` class) for the signature.

Read and understand:
- The method name, return type, and all parameters (names, types, defaults)
- Whether the method is on `Nunchuk` (instance method) or `Utils` (static utility)

### 2. Determine type mappings

Map C++ types to JNI/Kotlin types. Common mappings in this project:

| C++ type | JNI type | Kotlin type |
|---|---|---|
| `std::string` | `jstring` | `String` |
| `bool` | `jboolean` | `Boolean` |
| `int` | `jint` | `Int` |
| `int64_t` / `Amount` | `jlong` | `Long` |
| `std::vector<std::string>` | `jobject` (List) | `List<String>` |
| `std::vector<SingleSigner>` | `jobject` (List) | `List<SingleSigner>` |
| `std::vector<Wallet>` | `jobject` (List) | `List<Wallet>` |
| `SingleSigner` | `jobject` | `SingleSigner` |
| `Wallet` | `jobject` | `Wallet` |
| `Transaction` | `jobject` | `Transaction` |
| `MasterSigner` | `jobject` | `MasterSigner` |
| `Device` | `jobject` | `Device` |
| `UnspentOutput` | `jobject` | `UnspentOutput` |
| `BtcUri` | `jobject` | `BtcUri` |
| `std::map<K,V>` | `jobject` | `Map<K,V>` |
| `Chain` | `jint` (ordinal) | `Int` (ordinal of Chain enum) |
| `AddressType` | `jint` (ordinal) | `Int` (ordinal of AddressType enum) |
| `SignerType` | `jobject` | `SignerType` |
| `ExportFormat` | `jint` (ordinal) | `Int` (ordinal of ExportFormat enum) |
| `void` | `void` | `Unit` |

### 3. Check if new Serializer/Deserializer methods are needed

- Read `src/main/native/serializer.h` and `src/main/native/deserializer.h` to check what conversion methods already exist.
- If the method uses a C++ type as parameter that doesn't have an existing `Serializer::convert2C*` method, create one.
- If the method returns a C++ type that doesn't have an existing `Deserializer::convert2J*` method, create one.
- Add declarations in the `.h` files and implementations in the corresponding `.cpp` files.
- **Memory leak prevention**: When writing Serializer/Deserializer methods, follow these rules carefully:
  - Always call `env->DeleteLocalRef()` on any `jobject`, `jclass`, `jstring`, or `jarray` created via JNI calls (`NewObject`, `FindClass`, `NewStringUTF`, `GetObjectArrayElement`, `CallObjectMethod`, etc.) once you are done using them, especially inside loops.
  - Exception: Do NOT delete the final return value `jobject` — the caller owns it.
  - When iterating over Java collections (e.g. `List`, `Map`) in Serializer `convert2C*` methods, delete each element reference obtained from `GetObjectArrayElement` or `CallObjectMethod` after extracting its data.
  - When building Java collections in Deserializer `convert2J*` methods, delete each intermediate `jobject` (e.g. list items) after calling `SetObjectArrayElement` or `CallBooleanMethod` (List.add), since the container holds its own reference.
  - Cache `jclass` and `jmethodID` lookups with local variables at the top of the method — do not call `FindClass`/`GetMethodID` repeatedly inside loops. Delete `jclass` refs when done.
  - For `jstring` obtained via `GetStringUTFChars`, always call `ReleaseStringUTFChars` before deleting the `jstring` ref.
  - Study existing Serializer/Deserializer methods in `serializer.cpp` and `deserializer.cpp` to match the project's local ref management patterns.

### 4. Add Kotlin JNI external function in `LibNunchukAndroid.kt`

Add the `external fun` declaration in `src/main/java/com/nunchuk/android/nativelib/LibNunchukAndroid.kt`:

```kotlin
@Throws(NCNativeException::class)
external fun methodName(
    param1: Type1,
    param2: Type2,
): ReturnType
```

- Use the `@Throws(NCNativeException::class)` annotation.
- Kotlin parameter types should match the JNI mapping.
- Place it near related methods in the file.

### 5. Add public Kotlin wrapper in `NunchukNativeSdk.kt`

Add the public wrapper in `src/main/java/com/nunchuk/android/nativelib/NunchukNativeSdk.kt`:

```kotlin
@Throws(NCNativeException::class)
fun methodName(
    param1: Type1,
    param2: Type2 = defaultValue, // add defaults where the C++ method has them
) = nunchukAndroid.methodName(param1, param2)
```

- Default parameter values from the C++ signature should be specified here (not in LibNunchukAndroid).
- Delegate directly to `nunchukAndroid.methodName(...)`.
- Place it near related methods in the file.

### 6. Create C++ JNI implementation

Create the JNI function in the appropriate `*-jni.cpp` file under `src/main/native/`:

- **Utils methods** -> `util-jni.cpp`
- **Wallet methods** -> `wallet-jni.cpp`
- **Signer methods** -> `signer-jni.cpp`
- **Transaction methods** -> `transaction-jni.cpp`
- **Room/Matrix methods** -> `room-jni.cpp`, `room-wallet-jni.cpp`, `room-transaction-jni.cpp`
- **NFC/TapSigner methods** -> `tap-protocol-jni.cpp`
- **SatsCard methods** -> `satscard-jni.cpp`
- **MK4/ColdCard methods** -> `mk4.cpp`
- **Sync methods** -> `sync-jni.cpp`
- **Group wallet methods** -> `group-wallet-jni.cpp`
- **Init/Settings methods** -> `initializer-jni.cpp`

Follow this pattern:

```cpp
extern "C"
JNIEXPORT <jni_return_type> JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_<methodName>(JNIEnv *env, jobject thiz,
                                                                  <jni_params>) {
    try {
        // 1. Convert Java params to C++ using Serializer
        // 2. Call libnunchuk:
        //    - Utils methods: Utils::<MethodName>(...)
        //    - Instance methods: NunchukProvider::get()->nu-><MethodName>(...)
        // 3. Convert C++ result to Java using Deserializer and return
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return <error_return>; // JNI_FALSE, nullptr, etc.
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return <error_return>;
    }
}
```

Key conventions:
- Use `StringWrapper(env, jstring_var)` to convert `jstring` to `std::string`
- Use `env->NewStringUTF(str.c_str())` to return a `jstring`
- For void methods, catch blocks don't need a return statement
- Error return: `nullptr` for object types, `JNI_FALSE` for boolean, `0` for numeric

### 7. Add any new Kotlin model classes if needed

If the C++ method uses types not yet represented in Kotlin (`src/main/java/com/nunchuk/android/model/` and `src/main/java/com/nunchuk/android/type/`), create appropriate data classes or enums.
