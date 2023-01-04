# Nunchuk Android Native SDK
Nunchuk Android Native SDK is a wrapper for [libnunchuk](https://github.com/nunchuk-io/libnunchuk).

For more info on our products, please visit [our website](https://nunchuk.io/).

# Building the SDK
## Build requirements
Java: 11

NDK: 21.4.7075529

minSdkVersion: 21

## Dependencies
### 1. Xcode Command Line Tools

The Xcode Command Line Tools are a collection of build tools for macOS.
These tools must be installed in order to build Bitcoin Core from source.

To install, run the following command from your terminal:

``` bash
xcode-select --install
```

Upon running the command, you should see a popup appear.
Click on `Install` to continue the installation process.

### 2. Homebrew Package Manager

Homebrew is a package manager for macOS that allows one to install packages from the command line easily.
To install the Homebrew package manager, see: https://brew.sh

Note: If you run into issues while installing Homebrew or pulling packages, refer to [Homebrew's troubleshooting page](https://docs.brew.sh/Troubleshooting).

### 3. Brew Dependencies

Run the following from your terminal:

``` bash
brew install automake libtool boost pkg-config libevent
```

### 4. Pull submodules

```
pushd ${PWD}/src/main/native
git submodule add --force -b main https://gitlab.com/nunchuck/libnunchuk.git
git submodule update --init --recursive
sh .install_deps.sh arm64-v8a
popd
```

### 5. Export PATHs:
```
export JAVA_HOME=/Library/Java/JavaVirtualMachines/jdk-11.0.2.jdk/Contents/Home
export PATH=$JAVA_HOME/bin:$PATH
export ANDROID_SDK=/Users/${USER}/Library/Android/sdk
export ANDROID_SDK_HOME=/Users/${USER}/Library/Android/sdk
export ANDROID_SDK_ROOT=/Users/${USER}/Library/Android/sdk
export ANDROID_HOME=/Users/${USER}/Library/Android/sdk 
export PATH=$PATH:$ANDROID_HOME/tools:$ANDROID_HOME/platform-tools
export ANDROID_NDK_HOME=/Users/${USER}/Library/Android/sdk/ndk/21.4.7075529
```

## Building
```
./gradlew clean assembleArm64_v8aDebug --stacktrace
./gradlew publish
```
