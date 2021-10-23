#include <cstring>
#include <nunchuk.h>
#include <nunchukmatrix.h>
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
        const std::string &pass_phrase,
        const std::string &account_id,
        SendEventFunc send_event_func
) {
    nu = MakeNunchukForAccount(settings, pass_phrase, account_id);
    nuMatrix = MakeNunchukMatrixForAccount(settings, pass_phrase, account_id, send_event_func);
}