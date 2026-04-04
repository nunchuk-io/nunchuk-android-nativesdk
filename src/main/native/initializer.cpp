#include "initializer.h"

Initializer *Initializer::get() {
    static Initializer instance;
    return &instance;
}

JNIEnvGuard::JNIEnvGuard() : env_(nullptr), needsDetach_(false) {
    int envState = Initializer::get()->jvm->GetEnv((void **) &env_, JNI_VERSION_1_6);
    if (envState == JNI_EDETACHED) {
        JavaVMAttachArgs args{JNI_VERSION_1_6, nullptr, nullptr};
        if (Initializer::get()->jvm->AttachCurrentThread(&env_, &args) != 0) {
            syslog(LOG_DEBUG, "[JNI] JNIEnvGuard: Failed to attach thread");
            env_ = nullptr;
        } else {
            needsDetach_ = true;
        }
    } else if (envState != JNI_OK) {
        syslog(LOG_DEBUG, "[JNI] JNIEnvGuard: GetEnv failed with state %d", envState);
        env_ = nullptr;
    }
}

JNIEnvGuard::~JNIEnvGuard() {
    if (needsDetach_) {
        Initializer::get()->jvm->DetachCurrentThread();
    }
}
