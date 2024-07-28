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

pwd=$(pwd)
echo "pwd::$pwd"

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

export ANDROID_ABI="${1:-arm64-v8a}"
ANDROID_TARGET=""
BITCOIN_FOLDER=""
OPENSSL_FOLDER=""

parseArgs() {
  if [ "$ANDROID_ABI" = $ANDROID_ABI_ARMEABI_V7A ]; then
    ANDROID_TARGET=$ANDROID_TARGET_ARMEABI_V7A
    BITCOIN_FOLDER=contrib/bitcoin-arm8
    OPENSSL_FOLDER=contrib/openssl-arm8
  elif [ "$ANDROID_ABI" = $ANDROID_ABI_ARM64_V8A ]; then
    ANDROID_TARGET=$ANDROID_TARGET_ARM64_V8A
    BITCOIN_FOLDER=contrib/bitcoin
    OPENSSL_FOLDER=contrib/openssl
  elif [ "$ANDROID_ABI" = $ANDROID_ABI_X86_64 ]; then
    ANDROID_TARGET=$ANDROID_TARGET_X86_64
  elif [ "$ANDROID_ABI" = $ANDROID_ABI_X86 ]; then
    ANDROID_TARGET=$ANDROID_TARGET_X86
  else
    echo "Invalid ABI argument $ANDROID_ABI"
    exit 1
  fi
}
parseArgs

copyDirectoryForArmv7a() {
  if [ ! -d libnunchuk/contrib/bitcoin-arm8 ]; then
    git clone libnunchuk/contrib/bitcoin libnunchuk/contrib/bitcoin-arm8
  else
    dest_ver=$(cd libnunchuk/contrib/bitcoin-arm8 && git rev-parse --short HEAD)
    src_ver=$(cd libnunchuk/contrib/bitcoin && git rev-parse --short HEAD)
    if [ $dest_ver != $src_ver ]; then
      rm -rf libnunchuk/contrib/bitcoin-arm8
      git clone libnunchuk/contrib/bitcoin libnunchuk/contrib/bitcoin-arm8
    fi
  fi

  if [ ! -d libnunchuk/contrib/openssl-arm8 ]; then
    git clone libnunchuk/contrib/openssl libnunchuk/contrib/openssl-arm8
  else
    dest_ver=$(cd libnunchuk/contrib/openssl-arm8 && git rev-parse --short HEAD)
    src_ver=$(cd libnunchuk/contrib/openssl && git rev-parse --short HEAD)
    if [ $dest_ver != $src_ver ]; then
      rm -rf libnunchuk/contrib/openssl-arm8
      git clone libnunchuk/contrib/openssl libnunchuk/contrib/openssl-arm8
    fi
  fi
}

copyDirectoryForArmv7a

#########################################################################################
####                                Bitcoin Deps                                     ####
#########################################################################################
installBitcoinDeps() {
  abi=$ANDROID_ABI
  target=$ANDROID_TARGET

  echo "-------------------------------------------------------------------------------"
  echo "                     Installing deps for $abi $target                          "
  echo "-------------------------------------------------------------------------------"

  export TARGET=$target

  export AR=$TOOLCHAIN/bin/llvm-ar
  export CC=$TOOLCHAIN/bin/$TARGET$API-clang
  export AS=$CC
  export CXX=$TOOLCHAIN/bin/$TARGET$API-clang++
  export LD=$TOOLCHAIN/bin/ld
  export RANLIB=$TOOLCHAIN/bin/llvm-ranlib
  export STRIP=$TOOLCHAIN/bin/llvm-strip

  ANDROID_SDK=$ANDROID_SDK ANDROID_NDK=$ANDROID_NDK_HOME make HOST=$TARGET ANDROID_TOOLCHAIN_BIN=$TOOLCHAIN ANDROID_API_LEVEL=$API NO_QT=1 NO_ZMQ=1 NO_QR=1 NO_UPNP=1 -j $num_jobs
}
#
pushd "libnunchuk/$BITCOIN_FOLDER/depends" || exit
installBitcoinDeps
popd || exit

#########################################################################################
####                                 Bitcoin Core                                    ####
#########################################################################################
installBitcoinCore() {
  abi=$ANDROID_ABI
  target=$ANDROID_TARGET
  echo "-------------------------------------------------------------------------------"
  echo "                        Installing core for $abi $target                       "
  echo "-------------------------------------------------------------------------------"

  export TARGET=$target

  export AR=$TOOLCHAIN/bin/llvm-ar
  export CC=$TOOLCHAIN/bin/$TARGET$API-clang
  export AS=$CC
  export CXX=$TOOLCHAIN/bin/$TARGET$API-clang++
  export LD=$TOOLCHAIN/bin/ld
  export RANLIB=$TOOLCHAIN/bin/llvm-ranlib
  export STRIP=$TOOLCHAIN/bin/llvm-strip
  sh ./autogen.sh
  CONFIG_SITE="$PWD/depends/$ANDROID_TARGET/share/config.site" CFLAGS="-fPIC" CXXFLAGS="-fPIC" ./configure --prefix=$PWD/depends/$TARGET --without-gui --disable-zmq --with-miniupnpc=no --with-incompatible-bdb --disable-bench --disable-tests --enable-module-ecdh
  make -j $num_jobs
}

pushd "libnunchuk/$BITCOIN_FOLDER" || exit
installBitcoinCore
popd || exit


#########################################################################################
####                                 OpenSSL Lib                                     ####
#########################################################################################

applyOpenSSLPatches() {
  cmp -s ./patches/openssl-15-android.conf libnunchuk/$OPENSSL_FOLDER/Configurations/15-android.conf || cp ./patches/openssl-15-android.conf libnunchuk/$OPENSSL_FOLDER/Configurations/15-android.conf
}

applyOpenSSLPatches

installOpenSSL() {
  if [ "$ANDROID_ABI" = $ANDROID_ABI_ARM64_V8A ]; then
    abi="arm64"
  elif [ "$ANDROID_ABI" = $ANDROID_ABI_ARMEABI_V7A ]; then
    abi="arm"
  else
    abi="$ANDROID_ABI"
  fi

  target=$ANDROID_TARGET
  echo "-------------------------------------------------------------------------------"
  echo "                    Installing OpenSSL for $abi $target                        "
  echo "-------------------------------------------------------------------------------"

  export TARGET=$target

  export AR=$TOOLCHAIN/bin/llvm-ar
  export CC=$TOOLCHAIN/bin/$TARGET$API-clang
  export AS=$CC
  export CXX=$TOOLCHAIN/bin/$TARGET$API-clang++
  export LD=$TOOLCHAIN/bin/ld
  export RANLIB=$TOOLCHAIN/bin/llvm-ranlib
  export STRIP=$TOOLCHAIN/bin/llvm-strip
  PATH=$TOOLCHAIN/bin:$PATH
  ./Configure android-$abi -D__ANDROID_API__=$API
  make -j $num_jobs
}

pushd "libnunchuk/$OPENSSL_FOLDER" || exit
installOpenSSL
popd || exit

echo "done"
