#include "initializer.h"

Initializer *Initializer::get() {
    static Initializer instance;
    return &instance;
}
