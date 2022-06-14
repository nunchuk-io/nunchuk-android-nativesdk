Check build steps in .gitlab-ci.yml

java: 11
minsdk:
    arm64: 21
    arm-v7a: 24
ndk: 21.0.6113669

Example paths (MacOS):
    -------------------------------------------------------------------------------
    export JAVA_HOME=/Library/Java/JavaVirtualMachines/jdk-11.0.2.jdk/Contents/Home
    export PATH=$JAVA_HOME/bin:$PATH
    
    export ANDROID_SDK=/Users/${USER}/Library/Android/sdk
    export ANDROID_SDK_HOME=/Users/${USER}/Library/Android/sdk
    export ANDROID_SDK_ROOT=/Users/${USER}/Library/Android/sdk
    export ANDROID_HOME=/Users/${USER}/Library/Android/sdk
    export PATH=$PATH:$ANDROID_HOME/tools:$ANDROID_HOME/platform-tools
    
    export ANDROID_NDK_HOME=/Users/${USER}/Library/Android/sdk/ndk/21.0.6113669
    -------------------------------------------------------------------------------
