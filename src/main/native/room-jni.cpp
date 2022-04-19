#include <cstring>
#include <jni.h>
#include <nunchuk.h>
#include <nunchukmatrix.h>
#include "nunchukprovider.h"
#include "serializer.h"
#include "deserializer.h"

using namespace nunchuk;

extern "C"
JNIEXPORT void JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_sendErrorEvent(
        JNIEnv *env,
        jobject thiz,
        jstring room_id,
        jstring platform,
        jstring code,
        jstring message
) {
    try {
        NunchukProvider::get()->nuMatrix->SendErrorEvent(
                env->GetStringUTFChars(room_id, JNI_FALSE),
                env->GetStringUTFChars(platform, JNI_FALSE),
                env->GetStringUTFChars(code, JNI_FALSE),
                env->GetStringUTFChars(message, JNI_FALSE)
        );
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        env->ExceptionOccurred();
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        env->ExceptionOccurred();
    }
}
