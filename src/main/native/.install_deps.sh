#!/bin/bash
set -e

if [ -z "$ANDROID_SDK" ]; then
  echo "export the ANDROID_SDK environment variable"
  exit 1
fi

if [ -z "$ANDROID_NDK_HOME" ]; then
  echo "export the ANDROID_NDK_HOME environment variable"
  exit 1
fi

if [ "$(uname)" = "Darwin" ]; then
    export TOOLCHAIN=$ANDROID_NDK_HOME/toolchains/llvm/prebuilt/darwin-x86_64
else
    export TOOLCHAIN=$ANDROID_NDK_HOME/toolchains/llvm/prebuilt/linux-x86_64
fi

export API=24

export ANDROID_ABI_ARMEABI_V7A="armeabi-v7a"
export ANDROID_ABI_ARM64_V8A="arm64-v8a"
export ANDROID_ABI_X86_64="x86_64"
export ANDROID_ABI_X86="x86"

export ANDROID_TARGET_ARMEABI_V7A="armv7a-linux-androideabi"
export ANDROID_TARGET_ARM64_V8A="aarch64-linux-android"
export ANDROID_TARGET_X86_64="x86_64-linux-android"
export ANDROID_TARGET_X86="i686-linux-android"

num_jobs=4
if [ -f /proc/cpuinfo ]; then
   num_jobs=$(grep ^processor /proc/cpuinfo | wc -l)
fi

#########################################################################################
####                                Bitcoin Deps                                     ####
#########################################################################################
applyBitcoinDependsPatches() {
  cmp -s ./patches/libevent.mk libnunchuk/contrib/bitcoin/depends/packages/libevent.mk || cp ./patches/libevent.mk libnunchuk/contrib/bitcoin/depends/packages/libevent.mk
}

installBitcoinDeps() {
  abi=$1
  target=$2

  echo "-------------------------------------------------------------------------------"
  echo "                     Installing deps for $abi $target                          "
  echo "-------------------------------------------------------------------------------"

  export AR=$TOOLCHAIN/bin/llvm-ar
  export CC=$TOOLCHAIN/bin/$target$API-clang
  export AS=$CC
  export CXX=$TOOLCHAIN/bin/$target$API-clang++
  export LD=$TOOLCHAIN/bin/ld
  export RANLIB=$TOOLCHAIN/bin/llvm-ranlib
  export STRIP=$TOOLCHAIN/bin/llvm-strip

  ANDROID_SDK=$ANDROID_SDK ANDROID_NDK=$ANDROID_NDK_HOME make HOST=$target ANDROID_TOOLCHAIN_BIN=$TOOLCHAIN ANDROID_API_LEVEL=$API NO_QT=1 NO_ZMQ=1 NO_QR=1 NO_UPNP=1 NO_SQLITE=1 NO_BDB=1  NO_USDT=1 -j $num_jobs
}

applyBitcoinDependsPatches
pushd "libnunchuk/contrib/bitcoin/depends" || exit
installBitcoinDeps $ANDROID_ABI_ARMEABI_V7A $ANDROID_TARGET_ARMEABI_V7A
installBitcoinDeps $ANDROID_ABI_ARM64_V8A $ANDROID_TARGET_ARM64_V8A
popd || exit


#########################################################################################
####                                 OpenSSL Lib                                     ####
#########################################################################################

applyOpenSSLPatches() {
  cmp -s ./patches/openssl-15-android.conf libnunchuk/contrib/openssl/Configurations/15-android.conf || cp ./patches/openssl-15-android.conf libnunchuk/contrib/openssl/Configurations/15-android.conf
}

if [ "$(uname)" = "Linux" ]; then
  applyOpenSSLPatches
fi

installOpenSSL() {
  abi=$1
  target=$2
  openssl_abi=$3
  echo "-------------------------------------------------------------------------------"
  echo "                    Installing OpenSSL for $abi $target                        "
  echo "-------------------------------------------------------------------------------"

  export AR=$TOOLCHAIN/bin/llvm-ar
  export CC=$TOOLCHAIN/bin/$target$API-clang
  export AS=$CC
  export CXX=$TOOLCHAIN/bin/$target$API-clang++
  export LD=$TOOLCHAIN/bin/ld
  export RANLIB=$TOOLCHAIN/bin/llvm-ranlib
  export STRIP=$TOOLCHAIN/bin/llvm-strip
  PATH=$TOOLCHAIN/bin:$PATH
  ./Configure $openssl_abi -D__ANDROID_API__=$API --prefix="$PWD/$target"
  make clean
  make -j $num_jobs
  make install_dev
}

pushd "libnunchuk/contrib/openssl" || exit
installOpenSSL $ANDROID_ABI_ARMEABI_V7A $ANDROID_TARGET_ARMEABI_V7A "android-arm"
installOpenSSL $ANDROID_ABI_ARM64_V8A $ANDROID_TARGET_ARM64_V8A "android-arm64"
popd || exit

echo "done"
