#include <jni.h>
#include <iostream>
#include <string>
#include <syslog.h>

class Initializer;

// RAII guard: attaches the current thread to the JVM if needed, detaches on destruction.
class JNIEnvGuard {
public:
    JNIEnvGuard();
    ~JNIEnvGuard();

    JNIEnv *get() const { return env_; }
    explicit operator bool() const { return env_ != nullptr; }

    JNIEnvGuard(const JNIEnvGuard &) = delete;
    JNIEnvGuard &operator=(const JNIEnvGuard &) = delete;

private:
    JNIEnv *env_;
    bool needsDetach_;
};

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
