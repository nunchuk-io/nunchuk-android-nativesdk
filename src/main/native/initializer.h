#include <jni.h>
#include <iostream>
#include <string>

class Initializer {
    static Initializer *_instance;

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
    jclass groupMessageListenerClass;
    jmethodID groupSandboxListenerMethod;
    jmethodID groupMessageListenerMethod;
};
