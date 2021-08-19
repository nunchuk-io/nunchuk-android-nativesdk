#include <cstring>
#include <jni.h>
#include <nunchuk.h>
#include "nunchukprovider.h"
#include "serializer.h"
#include "deserializer.h"

using namespace nunchuk;

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
        jstring account_id,
        jobject send_event_executor
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

        jclass clazz = env->FindClass("com/nunchuk/android/model/SendEventExecutor");
        jmethodID method = env->GetMethodID(clazz, "execute", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");

        NunchukProvider::get()->initNunchuk(
                settings,
                env->GetStringUTFChars(pass_phrase, JNI_FALSE),
                env->GetStringUTFChars(account_id, JNI_FALSE),
                [&env, &send_event_executor, &method](std::string room_id, std::string type, std::string content) {
                    auto result = (jstring) env->CallObjectMethod(
                            send_event_executor,
                            method,
                            env->NewStringUTF(room_id.c_str()),
                            env->NewStringUTF(type.c_str()),
                            env->NewStringUTF(content.c_str())
                    );
                    return env->GetStringUTFChars(result, JNI_FALSE);
                }
        );
    } catch (const std::exception &e) {
        Deserializer::convert2JException(env, e.what());
        env->ExceptionOccurred();
    }
}

