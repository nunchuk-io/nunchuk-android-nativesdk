#include <cstring>
#include <jni.h>
#include <syslog.h>
#include <nunchuk.h>
#include <nunchukmatrix.h>

using namespace nunchuk;

class NFC {
public:
    static std::unique_ptr<tap_protocol::Transport> makeTransport(JNIEnv *env, jobject iso_dep);
};