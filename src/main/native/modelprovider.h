#include <cstring>
#include <jni.h>
#include <syslog.h>
#include <nunchuk.h>

using namespace nunchuk;

class ModelProvider {

public:

    static jobject createEmptyTransaction(JNIEnv *env);

    static jobject createEmptyWallet(JNIEnv *env);

    static jobject createEmptySingleSigner(JNIEnv *env);

    static jobject createEmptyMasterSigner(JNIEnv *env);

    static jobject createEmptyAmount(JNIEnv *env);

    static jobject newObject(JNIEnv *env, jclass clazz);

    static jobject createEmptyList(JNIEnv *env);

};