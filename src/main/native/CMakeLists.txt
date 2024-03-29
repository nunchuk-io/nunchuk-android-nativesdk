cmake_minimum_required(VERSION 3.4.1)
project(nunchuk-android)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(BITCOIN_FOLDER "contrib/bitcoin")
if (${ANDROID_ABI} STREQUAL "arm64-v8a")
    set(ANDROID_TARGET "aarch64-linux-android")
elseif (${ANDROID_ABI} STREQUAL "armeabi-v7a")
    set(ANDROID_TARGET "armv7a-linux-androideabi")
    set(BITCOIN_FOLDER "contrib/bitcoin-arm8")
else ()
    set(ANDROID_TARGET "${ANDROID_ABI}-linux-android")
endif ()

message("ANDROID_ABI::${ANDROID_ABI},ANDROID_TARGET::${ANDROID_TARGET}")

set(UR__DISABLE_TESTS ON)

add_library(nunchuk-android SHARED
        initializer.cpp
        deserializer.cpp
        serializer.cpp
        nfc.cpp
        nunchukprovider.cpp
        initializer-jni.cpp
        wallet-jni.cpp
        signer-jni.cpp
        room-wallet-jni.cpp
        room-jni.cpp
        room-transaction-jni.cpp
        transaction-jni.cpp
        util-jni.cpp
        modelprovider.cpp
        sync-jni.cpp
        tap-protocol-jni.cpp
        satscard-jni.cpp
        mk4.cpp
        string-wrapper.h
)

set(UR__DISABLE_TESTS ON)
add_subdirectory(libnunchuk)

target_link_libraries(nunchuk-android
        nunchuk)

target_include_directories(nunchuk-android PUBLIC
        "${PROJECT_SOURCE_DIR}"
        "${PROJECT_SOURCE_DIR}/libnunchuk/include"
        "${PROJECT_SOURCE_DIR}/${BITCOIN_FOLDER}/src"
        "${PROJECT_SOURCE_DIR}/${BITCOIN_FOLDER}/src/univalue/include"
        "${PROJECT_SOURCE_DIR}/${BITCOIN_FOLDER}/src/leveldb/include"
        "${PROJECT_SOURCE_DIR}/${BITCOIN_FOLDER}/depends/${ANDROID_TARGET}/include")
