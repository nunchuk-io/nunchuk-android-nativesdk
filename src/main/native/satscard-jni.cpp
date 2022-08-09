#include <jni.h>
#include <string>
#include <tap_protocol/tap_protocol.h>
#include <tap_protocol/cktapcard.h>
#include <iostream>
#include <android/log.h>
#include <syslog.h>
#include "deserializer.h"
#include "serializer.h"
#include "nunchukprovider.h"
#include "nfc.h"
#include "string-wrapper.h"

using namespace nunchuk;

extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_getSatsCardStatus(JNIEnv *env, jobject thiz, jobject iso_dep) {
    try {
        auto ts = NunchukProvider::get()->nu->CreateSatscard(NFC::makeTransport(env, iso_dep));
        auto status = NunchukProvider::get()->nu->GetSatscardStatus(ts.get());
        return Deserializer::convert2JSatsCardStatus(env, status);
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return JNI_FALSE;
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return JNI_FALSE;
    }
}
extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_setupSatsCard(JNIEnv *env, jobject thiz, jobject iso_dep, jstring cvc, jstring chain_code) {
    try {
        auto ts = NunchukProvider::get()->nu->CreateSatscard(NFC::makeTransport(env, iso_dep));
        auto status = NunchukProvider::get()->nu->SetupSatscard(
                ts.get(),
                env->GetStringUTFChars(cvc, JNI_FALSE),
                env->GetStringUTFChars(chain_code, JNI_FALSE)
        );
        return Deserializer::convert2JSatsCardStatus(env, status);
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return JNI_FALSE;
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return JNI_FALSE;
    }
}
extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_loadSatsCardSlotsBalance(JNIEnv *env, jobject thiz, jobject slots) {
    try {
        auto sats_card_slots = Serializer::convert2CSatsCardSlots(env, slots);
        for (auto &slot: sats_card_slots) {
            if (slot.get_status() != SatscardSlot::Status::UNUSED) {
                slot = NunchukProvider::get()->nu->FetchSatscardSlotUTXOs(slot);
            }
        }
        return Deserializer::convert2JSatsCardSlots(env, sats_card_slots);
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return JNI_FALSE;
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return JNI_FALSE;
    }
}
extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_unsealSatsCard(JNIEnv *env, jobject thiz, jobject iso_dep, jstring cvc, jobject jSlot) {
    try {
        auto ts = NunchukProvider::get()->nu->CreateSatscard(NFC::makeTransport(env, iso_dep));
        auto slot = Serializer::convert2CSatsCardSlot(env, jSlot);
        auto returnSlot = NunchukProvider::get()->nu->UnsealSatscard(ts.get(), env->GetStringUTFChars(cvc, JNI_FALSE), slot);
        return Deserializer::convert2JSatsCardSlot(env, returnSlot);
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return JNI_FALSE;
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return JNI_FALSE;
    }
}
extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_sweepBalance(JNIEnv *env, jobject thiz, jobject slots, jstring address, jint fee_rate) {
    try {
        auto sats_card_slots = Serializer::convert2CSatsCardSlots(env, slots);
        auto tx = NunchukProvider::get()->nu->SweepSatscardSlots(sats_card_slots, StringWrapper(env, address), fee_rate);
        return Deserializer::convert2JTransaction(env, tx);
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return JNI_FALSE;
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return JNI_FALSE;
    }
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_getSlotKeys(JNIEnv *env, jobject thiz, jobject iso_dep, jstring cvc, jobject slots) {
    try {
        auto sats_card_slots = Serializer::convert2CSatsCardSlots(env, slots);
        auto ts = NunchukProvider::get()->nu->CreateSatscard(NFC::makeTransport(env, iso_dep));
        auto cvcChars = env->GetStringUTFChars(cvc, JNI_FALSE);
        std::string cvcVal(cvcChars);
        env->ReleaseStringUTFChars(cvc, cvcChars);
        for (auto &slot: sats_card_slots) {
            slot = NunchukProvider::get()->nu->GetSatscardSlotKey(ts.get(), cvcVal, slot);
        }
        return Deserializer::convert2JSatsCardSlots(env, sats_card_slots);
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return JNI_FALSE;
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return JNI_FALSE;
    }
}
extern "C"
JNIEXPORT jobject JNICALL
Java_com_nunchuk_android_nativelib_LibNunchukAndroid_draftSatscardTransaction(JNIEnv *env, jobject thiz, jobject slots, jstring address,
                                                                              jint fee_rate) {
    try {
        auto sats_card_slots = Serializer::convert2CSatsCardSlots(env, slots);
        auto tx = NunchukProvider::get()->nu->CreateSatscardSlotsTransaction(sats_card_slots, StringWrapper(env, address), fee_rate);
        return Deserializer::convert2JTransaction(env, tx);
    } catch (BaseException &e) {
        Deserializer::convert2JException(env, e);
        return JNI_FALSE;
    } catch (std::exception &e) {
        Deserializer::convertStdException2JException(env, e);
        return JNI_FALSE;
    }
}