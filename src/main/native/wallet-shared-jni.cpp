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
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_initWallet(
        JNIEnv *env,
        jobject thiz,
        jstring room_id,
        jstring name,
        jint m,
        jint n,
        jint address_type,
        jboolean is_escrow
) {
    syslog(LOG_DEBUG, "[JNI]initWallet()");
    try {
        AddressType type = Serializer::convert2CAddressType(address_type);
        syslog(LOG_DEBUG, "[JNI]roomId::%s", env->GetStringUTFChars(room_id, JNI_FALSE));
        syslog(LOG_DEBUG, "[JNI]name::%s", env->GetStringUTFChars(name, JNI_FALSE));
        syslog(LOG_DEBUG, "[JNI]m::%d", m);
        syslog(LOG_DEBUG, "[JNI]n::%d", n);
        syslog(LOG_DEBUG, "[JNI]address_type::%d", address_type);
        syslog(LOG_DEBUG, "[JNI]is_escrow::%d", is_escrow);
        NunchukProvider::get()->nuMatrix->InitWallet(
                env->GetStringUTFChars(room_id, JNI_FALSE),
                env->GetStringUTFChars(name, JNI_FALSE),
                m,
                n,
                type,
                is_escrow
        );
    } catch (std::exception &e) {
        syslog(LOG_DEBUG, "[JNI] initWallet error::%s", e.what());
        Deserializer::convert2JException(env, e.what());
        env->ExceptionOccurred();
    }
}

