#include <jni.h>
#include <iostream>
#include <string>

class Initializer {
private:
    Initializer() {}

public:
    static Initializer *get();

    JavaVM *jvm;
    jclass sendEventClass;
    jmethodID senEventMethod;

    jclass sendFileClass;
    jmethodID sendFileMethod;

    jclass downloadFileClass;
    jmethodID downloadFileMethod;

    jclass syncFileClass;
    jmethodID syncFileMethod;

    jclass connectStatusClass;
    jmethodID connectStatusMethod;

    jclass blockListenerClass;
    jmethodID blockListenerMethod;

    jclass transactionListenerClass;
    jmethodID transactionListenerMethod;

    jclass groupSandboxListenerClass;
    jmethodID groupSandboxListenerMethod;

    jclass groupMessageListenerClass;
    jmethodID groupMessageListenerMethod;

    jclass groupOnlineListenerClass;
    jmethodID groupOnlineListenerMethod;

    jclass groupDeleteListenerClass;
    jmethodID groupDeleteListenerMethod;

    jclass groupReplaceListenerClass;
    jmethodID groupReplaceListenerMethod;

    jobject classLoader;
    jmethodID loadClassMethod;

    jclass getClass(JNIEnv *env, const char *classPath);
};
