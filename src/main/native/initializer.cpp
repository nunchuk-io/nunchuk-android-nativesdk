#include "initializer.h"

Initializer *Initializer::get() {
    if (!_instance) {
        _instance = new Initializer;
    }
    return _instance;
}

Initializer *Initializer::_instance = nullptr;
