#include <cstring>
#include <jni.h>
#include <nunchuk.h>
#include "modelprovider.h"

using namespace nunchuk;

jobject ModelProvider::createEmptyTransaction(JNIEnv *env) {
    jclass clazz = env->FindClass("com/nunchuk/android/model/Transaction");
    return newObject(env, clazz);
}

jobject ModelProvider::createEmptyWallet(JNIEnv *env) {
    jclass clazz = env->FindClass("com/nunchuk/android/model/Wallet");
    return newObject(env, clazz);
}

jobject ModelProvider::createEmptySingleSigner(JNIEnv *env) {
    jclass clazz = env->FindClass("com/nunchuk/android/model/SingleSigner");
    return newObject(env, clazz);
}

jobject ModelProvider::createEmptyMasterSigner(JNIEnv *env) {
    jclass clazz = env->FindClass("com/nunchuk/android/model/MasterSigner");
    return newObject(env, clazz);
}

jobject ModelProvider::createEmptyAmount(JNIEnv *env) {
    jclass clazz = env->FindClass("com/nunchuk/android/model/Amount");
    return newObject(env, clazz);
}

jobject ModelProvider::newObject(JNIEnv *env, jclass clazz) {
    jmethodID constructor = env->GetMethodID(clazz, "<init>", "()V");
    jobject instance = env->NewObject(clazz, constructor);
    return instance;
}

jobject ModelProvider::createEmptyList(JNIEnv *env) {
    static auto arrayListClass = static_cast<jclass>(env->NewGlobalRef(env->FindClass("java/util/ArrayList")));
    static jmethodID constructor = env->GetMethodID(arrayListClass, "<init>", "()V");
    jobject arrayListInstance = env->NewObject(arrayListClass, constructor);
    jmethodID addMethod = env->GetMethodID(arrayListClass, "add", "(Ljava/lang/Object;)Z");
    return arrayListInstance;
}