#include "state/State.hpp"
#include "controllers/MainController.hpp"

CanState& State::getCanState()
{
    return _can_state;
}

RotateState& State::getRotateState()
{
    return _rotate_state;
}

MotorState& State::getMotorState()
{
    return _motor_state;
}

EjectionState& State::getEjectionState()
{
    return _ejection_state;
}

int State::setActuatorValue(int index, int value)
{
    snowdebug("Setting actuator %d to value %d\n", index, value);
    if (index == MainController::ROTATE_ACTUATOR_INDEX) {
        _rotate_state.setTargetAngle(value);
        return 0;
    } else if (index == MainController::ESC_ACTUATOR_INDEX) {
        _motor_state.setValue(value);
        return 0;
    } else if (index == MainController::ANGLE_ACTUATOR_INDEX) {
        _ejection_state.setAngle(value);
        return 0;
    }
    return -1; // Return -1 if index is invalid
}

State::State () {
}

State::~State() {
    
}

bool State::getIsEnabled() {
    return _is_enabled;
}