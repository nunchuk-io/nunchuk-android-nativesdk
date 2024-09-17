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
        const std::string &device_id,
        const std::string &decoy_pin,
        SendEventFunc send_event_func
) {
    // if decoy pin is empty use MakeNunchukForAccount else use MakeNunchukForDecoyPin
    nu = decoy_pin.empty() ? MakeNunchukForAccount(settings, pass_phrase, account_id)
                           : MakeNunchukForDecoyPin(settings, decoy_pin);
    nuMatrix = MakeNunchukMatrixForAccount(settings, pass_phrase, account_id, device_id, send_event_func);
}