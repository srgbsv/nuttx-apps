#include "Config.hpp"
#include "state/MotorState.hpp"
#include "Logging.hpp"

int MotorState::getValue() { 
    return _value; 
}

void MotorState::setValue(int value) {
    if (value != _value) {
        snowinfo("ESC value set to %d\n", value);
        _value = value; 
    }
}
