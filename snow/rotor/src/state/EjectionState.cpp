
#include "Config.hpp"
#include "state/EjectionState.hpp"
#include "Logging.hpp"

int EjectionState::getAngle() { 
    return _angle; 
}

void EjectionState::setAngle(int angle) { 
    if (angle != _angle) {
        snowinfo("Throw angle set to %d", angle);
        _angle = angle; 
    }
}
