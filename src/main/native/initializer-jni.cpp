#include <cstring>
#include <jni.h>
#include <nunchuk.h>
#include "nunchukprovider.h"
#include "serializer.h"
#include "deserializer.h"

using namespace nunchuk;

static JavaVM *jvm = nullptr;
static jclass callbackClass;
static jmethodID callbackMethod;

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    syslog(LOG_DEBUG, "[JNI]JNI_OnLoad()");
    JNIEnv *env;
    vm->GetEnv((void **) &env, JNI_VERSION_1_6);
    syslog(LOG_DEBUG, "[JNI]Init env");
    jclass tmpClass = env->FindClass("com/nunchuk/android/model/SendEventHelper");
    syslog(LOG_DEBUG, "[JNI]Init class");
    jmethodID tmpMethod = env->GetStaticMethodID(tmpClass, "sendEvent", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
    syslog(LOG_DEBUG, "[JNI]Init method");
    callbackClass = (jclass) env->NewGlobalRef(tmpClass);
    syslog(LOG_DEBUG, "[JNI]Store class");
    callbackMethod = tmpMethod;
    syslog(LOG_DEBUG, "[JNI]Store method");
    return JNI_VERSION_1_6;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_initNunchuk(
        JNIEnv *env,
        jobject thiz,
        jint chain,
        jstring hwi_path,
        jboolean enable_proxy,
        jobject testnet_servers,
        jint backend_type,
        jstring storage_path,
        jstring pass_phrase,
        jstring account_id
) {
    try {
        AppSettings settings;
        settings.set_chain(Serializer::convert2CChain(chain));
        settings.set_hwi_path(env->GetStringUTFChars(hwi_path, JNI_FALSE));
        settings.enable_proxy(enable_proxy);
        settings.set_testnet_servers(Serializer::convert2CListString(env, testnet_servers));
        settings.set_backend_type(Serializer::convert2CBackendType(backend_type));
        settings.set_storage_path(env->GetStringUTFChars(storage_path, JNI_FALSE));

        env->GetJavaVM(&jvm);
        NunchukProvider::get()->initNunchuk(
                settings,
                env->GetStringUTFChars(pass_phrase, JNI_FALSE),
                env->GetStringUTFChars(account_id, JNI_FALSE),
                [](const std::string &room_id, const std::string &type, const std::string &content) {
                    syslog(LOG_DEBUG, "[JNI]send_event_func()");
                    syslog(LOG_DEBUG, "[JNI]room_id::%s", room_id.c_str());
                    syslog(LOG_DEBUG, "[JNI]type::%s", type.c_str());
                    syslog(LOG_DEBUG, "[JNI]content::%s", content.c_str());
                    JNIEnv *g_env;
                    try {
                        JavaVMAttachArgs args;
                        args.version = JNI_VERSION_1_6;
                        args.name = nullptr;
                        args.group = nullptr;
                        jvm->GetEnv((void **) &g_env, JNI_VERSION_1_6);
                        int envState = jvm->GetEnv((void **) &g_env, JNI_VERSION_1_6);
                        if (envState == JNI_EDETACHED) {
                            syslog(LOG_DEBUG, "[JNI]GetEnv: not attached\n");
                            if (jvm->AttachCurrentThread(&g_env, &args) != 0) {
                                syslog(LOG_DEBUG, "[JNI]GetEnv: Failed to attach\n");
                            } else {
                                syslog(LOG_DEBUG, "[JNI]GetEnv: Attached to current thread\n");
                            }
                        } else if (envState == JNI_OK) {
                            syslog(LOG_DEBUG, "[JNI]GetEnv: JNI_OK\n");
                        } else if (envState == JNI_EVERSION) {
                            syslog(LOG_DEBUG, "[JNI]GetEnv: version not supported\n");
                        }
                        syslog(LOG_DEBUG, "[JNI]CallStaticVoidMethod");
                        g_env->CallStaticVoidMethod(
                                callbackClass,
                                callbackMethod,
                                g_env->NewStringUTF(room_id.c_str()),
                                g_env->NewStringUTF(type.c_str()),
                                g_env->NewStringUTF(content.c_str())
                        );
                    } catch (const std::exception &t) {
                        Deserializer::convert2JException(g_env, t.what());
                        g_env->ExceptionOccurred();
                    }
                    return "";
                }
        );
        NunchukProvider::get()->nuMatrix->EnableGenerateReceiveEvent(NunchukProvider::get()->nu);
    } catch (const std::exception &e) {
        Deserializer::convert2JException(env, e.what());
        env->ExceptionOccurred();
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_enableGenerateReceiveEvent(
        JNIEnv *env,
        jobject thiz
) {
}