#include <cstring>
#include <jni.h>
#include <syslog.h>
#include <nunchuk.h>
#include <nunchukmatrix.h>

using namespace nunchuk;

class NunchukProvider {
    static NunchukProvider *_instance;

private:
    NunchukProvider() {
        syslog(LOG_DEBUG, "[JNI] Created NunchukProvider");
    }

public:
    static NunchukProvider *get();

    std::unique_ptr<Nunchuk> nu;
    std::unique_ptr<Utils> nuUtils;
    std::unique_ptr<NunchukMatrix> nuMatrix;

    void initNunchuk(
            const AppSettings &settings,
            const std::string &pass_phrase,
            const std::string &account_id,
            const std::string &device_id,
            const std::string &decoy_pin,
            SendEventFunc send_event_func
    );
};