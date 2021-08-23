#include <cstring>
#include <jni.h>
#include <nunchuk.h>
#include "nunchukprovider.h"
#include "serializer.h"
#include "deserializer.h"

using namespace nunchuk;

static JavaVM *jvm = NULL;
JNIEnv *store_env;

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
        // FIXME
        settings.set_testnet_servers({"testnet.nunchuk.io:50001"});
        settings.set_backend_type(Serializer::convert2CBackendType(backend_type));
        settings.set_storage_path(env->GetStringUTFChars(storage_path, JNI_FALSE));

        env->GetJavaVM(&jvm);
        store_env = env;
        NunchukProvider::get()->initNunchuk(
                settings,
                env->GetStringUTFChars(pass_phrase, JNI_FALSE),
                env->GetStringUTFChars(account_id, JNI_FALSE),
                [](std::string room_id, std::string type, std::string content) {
                    JNIEnv *g_env;
                    try {
                        JavaVMAttachArgs args;
                        args.version = JNI_VERSION_1_6;
                        args.name = nullptr;
                        args.group = nullptr;

                        jvm->GetEnv((void **) &g_env, JNI_VERSION_1_6);
                        jclass clazz = g_env->FindClass("com/nunchuk/android/model/SendEventHelper");
                        jmethodID method = g_env->GetStaticMethodID(clazz, "sendEvent", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
                        g_env->CallStaticVoidMethod(
                                clazz,
                                method,
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
    } catch (const std::exception &e) {
        Deserializer::convert2JException(env, e.what());
        env->ExceptionOccurred();
    }
}