#include <cstring>
#include <jni.h>
#include <syslog.h>
#include <nunchuk.h>
#include "nunchukprovider.h"

using namespace nunchuk;

NunchukProvider *NunchukProvider::get() {
    if (!_instance) {
        _instance = new NunchukProvider;
    }
    return _instance;
}

NunchukProvider *NunchukProvider::_instance = nullptr;

void NunchukProvider::initNunchuk(
    const AppSettings &settings,
    const std::string& pass_phrase,
    const std::string& account_id
) {
    nu = MakeNunchukForAccount(settings, pass_phrase, account_id);
}