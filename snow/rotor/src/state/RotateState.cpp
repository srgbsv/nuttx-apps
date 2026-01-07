#include "state/RotateState.hpp"
#include "Logging.hpp"

void RotateState::setAngle(int angle) {
    if (angle != _angle) {
        snowinfo("Rotate angle set to %d", angle);
        _angle = angle;
    }
}

int RotateState::getAngle() { 
    return _angle; 
}

void RotateState::setTargetAngle(int angle) { 
    if (angle != _target_angle) {
        snowinfo("Rotate target angle set to %d\n", angle);
    }
    _target_angle = angle; 
}

int RotateState::getTargetAngle() { 
    return _target_angle; 
}
