//
// Created by sergey on 11.10.24.
//

#include "../include/InputController.h"
#include "syslog.h"
#include "config.h"

InputController::InputController (
    const char* _motor_enable_dev,
    const char* _rotation_dev,
    const char* _ejection_dev,
    const char* _general_switch_dev) {
    init (_motor_enable_dev, _rotation_dev, _ejection_dev, _general_switch_dev);
}

bool InputController::init(
    const char* _motor_enable_dev,
    const char* _rotation_dev,
    const char* _ejection_dev,
    const char* _general_switch_dev
) {
    _pwm_input_motor.init(_motor_enable_dev, 30, 100);
    if (!_pwm_input_motor.isInit ()) {
        syslog(LOG_ERR, "Can't input pwm device %s", _motor_enable_dev);
    } else {
#ifdef SNOW_DEBUG
        syslog(LOG_DEBUG, "InputController: device %s PWM inited", _motor_enable_dev);
#endif
    }
    _pwm_input_rotation.init(_rotation_dev, 30, 100);
    if (!_pwm_input_rotation.isInit ()) {
        syslog(LOG_ERR, "Can't input pwm device %s", _rotation_dev);
    } else {
#ifdef SNOW_DEBUG
        syslog(LOG_DEBUG, "InputController: device %s PWM inited", _rotation_dev);
#endif
    }
    _pwm_input_ejection.init(_ejection_dev, 30, 100);
    if (!_pwm_input_ejection.isInit()) {
        syslog(LOG_ERR, "Can't input pwm device %s", _ejection_dev);
    } else {
#ifdef SNOW_DEBUG
        syslog(LOG_DEBUG, "InputController: device %s PWM inited", _ejection_dev);
#endif
    }
    _pwm_input_switch.init(_general_switch_dev, 30, 100);
    if (!_pwm_input_switch.isInit()) {
        syslog(LOG_ERR, "Can't input pwm device %s", _general_switch_dev);
    } else {
#ifdef SNOW_DEBUG
        syslog(LOG_DEBUG, "InputController: device %s PWM inited", _general_switch_dev);
#endif
    }

    return true;
}

int InputController::getMotorValue() {
    auto motor_pwm = _pwm_input_motor.getDutyCycleNormalized();
    if (motor_pwm < 0 || motor_pwm > 100) {
        //Invalid PWM input
        return -1;
    } 
    if (motor_pwm > 30 && motor_pwm <75) {
        return 12;
    }
    if (motor_pwm >= 65) {
        return 32;
    }
    return 2;
}

int InputController::getMotorValueRaw () {
    return _pwm_input_motor.getDutyCycleNormalized();
}

int InputController::getRotationValue() {
    auto rotation_pwm = _pwm_input_rotation.getDutyCycleNormalized();
    if (rotation_pwm < 0 || rotation_pwm > 100) {
        //Invalid PWM input
        return 0;
    } 
    if (rotation_pwm > 75) return 1;
    if (rotation_pwm < 25) return -1;
    return 0;
}

int InputController::getRotationValueRaw () {
    return _pwm_input_rotation.getDutyCycle();
}

int InputController::getEjectionValue() {
    auto ejection_pwm =  _pwm_input_ejection.getDutyCycleNormalized();
    if (ejection_pwm < 0 || ejection_pwm > 100) {
        return -1;
    }
    if (ejection_pwm > 70) return 3;
    if (ejection_pwm < 40) return 8; 
    return 5;
}

int InputController::getEjectionValueRaw () {
    return _pwm_input_ejection.getDutyCycle();
}

int InputController::getSwitchValue() {
    auto switch_value = _pwm_input_switch.getDutyCycleNormalized();
    if (switch_value < 0 || switch_value > 100) {
        return -1;
    }
    return switch_value < 50;
}
