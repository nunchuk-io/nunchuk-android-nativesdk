#ifndef STRING_WRAPPER_H
#define STRING_WRAPPER_H

#include <jni.h>
#include <string>
#include <utility>

class StringWrapper {
public:
    StringWrapper() = default;

    explicit StringWrapper(const std::string &str) : str_(str) {};

    explicit StringWrapper(std::string &&str) : str_(std::move(str)) {};

    StringWrapper(JNIEnv *env, jstring str) {
        const char *ch = env->GetStringUTFChars(str, nullptr);
        str_ = std::string(ch);
        env->ReleaseStringUTFChars(str, ch);
    }

    operator std::string &() { return str_; }

    operator std::string const &() const { return str_; }

    jstring toJString(JNIEnv *env) const {
        jstring jstr = env->NewStringUTF(str_.c_str());
        return jstr;
    }

private:
    std::string str_;
};

#endif