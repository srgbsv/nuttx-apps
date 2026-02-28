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
    lock();
    //snowdebug("Setting actuator %d to value %d\n", index, value);
    if (index == MainController::ROTATE_ACTUATOR_INDEX) {
        _rotate_state.setTargetAngle(value);
        unlock();
        return 0;
    } else if (index == MainController::ESC_ACTUATOR_INDEX) {
        _motor_state.setValue(value);
        unlock();
        return 0;
    } else if (index == MainController::ANGLE_ACTUATOR_INDEX) {
        _ejection_state.setAngle(value);
        unlock();
        return 0;
    }
    unlock();
    return -1; // Return -1 if index is invalid
}

State::State () {
    pthread_mutex_init(&_state_mutex, nullptr);
}

State::~State() {
    pthread_mutex_destroy(&_state_mutex);
}

bool State::getIsEnabled() {
    return _is_enabled;
}