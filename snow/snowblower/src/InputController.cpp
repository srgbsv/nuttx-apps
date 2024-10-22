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
    const char* _test_btn_dev) {
    init (_motor_enable_dev, _rotation_dev, _ejection_dev, _test_btn_dev);
}

bool InputController::init(
    const char* _motor_enable_dev,
    const char* _rotation_dev,
    const char* _ejection_dev,
    const char* _test_btn_dev
) {
    _pwm_input_motor.init(_motor_enable_dev);
    if (!_pwm_input_motor.isInit ()) {
        syslog(LOG_ERR, "Can't input pwm device %s", _motor_enable_dev);
    } else {
#ifdef SNOW_DEBUG
        syslog(LOG_DEBUG, "InputController: device %s PWM inited", _motor_enable_dev);
#endif
    }
    _pwm_input_rotation.init(_rotation_dev);
    if (!_pwm_input_rotation.isInit ()) {
        syslog(LOG_ERR, "Can't input pwm device %s", _rotation_dev);
    } else {
#ifdef SNOW_DEBUG
        syslog(LOG_DEBUG, "InputController: device %s PWM inited", _rotation_dev);
#endif
    }
    _pwm_input_ejection.init(_ejection_dev);
    if (!_pwm_input_ejection.isInit()) {
        syslog(LOG_ERR, "Can't input pwm device %s", _ejection_dev);
    } else {
#ifdef SNOW_DEBUG
        syslog(LOG_DEBUG, "InputController: device %s PWM inited", _ejection_dev);
#endif
    }
    _gpio_in_test_btn.init(_test_btn_dev);
    if (!_gpio_in_test_btn.isInit()) {
        syslog(LOG_ERR, "Can't input gpio device %s", _test_btn_dev);
    } else {
#ifdef SNOW_DEBUG
        syslog(LOG_DEBUG, "InputController: device %s GPIO inited", _test_btn_dev);
#endif
    }

    return true;
}

int InputController::getMotorValue() {
    return _pwm_input_motor.getDutyCycle();
}
int InputController::getRotationValue() {
    return _pwm_input_rotation.getDutyCycle();
}
int InputController::getEjectionValue() {
    return _pwm_input_ejection.getDutyCycle();
}
bool InputController::isTestBtnPressed() {
    return _gpio_in_test_btn.getState();
}
