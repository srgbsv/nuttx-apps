//
// Created by sergey on 20.07.24.
//

#include "../include/EjectionController.h"
#include <fcntl.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <syslog.h>

#include <nuttx/ioexpander/gpio.h>
#include <nuttx/config.h>
#include "EjectionController.hpp"

#define GPIOC_WRITE 1

/*bool EjectionController::init (const char* direction_gpio_dev, const char* rotation_gpio_dev, const char* motor_enable_gpio_dev, const char* angle_gpio_dev) {

}*/

EjectionController::EjectionController () {
}

void EjectionController::setAngle (float angle) {
    if (_current_angle != angle) {
        _current_angle = angle;
        _angle_current_cnt = 0;
        syslog(LOG_DEBUG, "EjectionController: changing angle value: %.2f -> %.2f", _current_angle, angle);
    } else {
        _angle_current_cnt++;
        if (_angle_current_cnt > _event_count_thd) { 
            forceAngleSet(_current_angle);
        }
    }
}

void EjectionController::setRotationEnable (bool run, bool clockwise){
    if (run != _rotation_enable || _rotation_direction != clockwise) {
        _rotation_enable = run;
        _rotation_direction = clockwise;
        _rotation_event_cnt = 0;
        syslog(LOG_DEBUG, "EjectionController: changing rotation value: %d %d -> %d %d ", 
            _rotation_enable, _rotation_direction, run, clockwise);
    } else {
        _rotation_event_cnt++;
        if (_rotation_event_cnt > _event_count_thd) {
            forceRotationSet(_rotation_enable, _rotation_direction);
        }
    }
}

void EjectionController::setMotor (float motor_value) {
    if (_current_motor_value != motor_value) {
        _current_motor_value = motor_value;
        _motor_value_cnt = 0;
        syslog(LOG_DEBUG, "EjectionController: changing motor value: %.2f -> %.2f", _current_motor_value, motor_value);
    } else {
        _motor_value_cnt++;
        if (_motor_value_cnt > _event_count_thd) {
            forceMotorSet(_current_motor_value);
        }
    }
}

void EjectionController::setEnable (bool switch_value) {
    if (_current_switch_value != switch_value) {
        _current_switch_value = switch_value;
        _switch_value_cnt = 0;
        syslog(LOG_DEBUG, "EjectionController: changing switch value: %d -> %d", _current_switch_value, switch_value);
    } else {
        _switch_value_cnt++;
        if (_switch_value_cnt > _event_count_thd) {
            forceSetEnable (_current_switch_value);
        }
    }
}

EjectionController::~EjectionController () {

}

bool EjectionController::init (const char* direction_gpio_dev,
    const char* rotation_gpio_dev,
    const char* motor_enable_gpio_dev,
    const char* angle_gpio_dev,
    const char* general_switch_gpio_dev
) {
    _direction_gpio.init (direction_gpio_dev);
    if (!_direction_gpio.isInit()) {
        syslog(LOG_ERR, "Unable to init output GPIO device %s", direction_gpio_dev);
    } else {
#ifdef SNOW_DEBUG
        syslog(LOG_DEBUG, "EjectionController: device %s GPIO inited", direction_gpio_dev);
#endif
    }
    _rotation_gpio.init (rotation_gpio_dev);
    if (!_rotation_gpio.isInit()) {
        syslog(LOG_ERR, "Unable to init output GPIO device %s", rotation_gpio_dev);
    } else {
#ifdef SNOW_DEBUG
        syslog(LOG_DEBUG, "EjectionController: device %s GPIO inited", rotation_gpio_dev);
#endif
    }
    _motor_pwm.init (motor_enable_gpio_dev);
    if (!_motor_pwm.isInit()) {
        syslog(LOG_ERR, "Unable to init output PWM device %s", motor_enable_gpio_dev);
    } else {
#ifdef SNOW_DEBUG
        syslog(LOG_DEBUG, "EjectionController: device %s PWM inited", motor_enable_gpio_dev);
#endif
    }
    _angle_pwm.init (angle_gpio_dev);
    if (!_angle_pwm.isInit()) {
        syslog(LOG_ERR, "Unable to init output PWM device %s", angle_gpio_dev);
    } else {
#ifdef SNOW_DEBUG
        syslog(LOG_DEBUG, "EjectionController: device %s PWM inited", angle_gpio_dev);
#endif
    }
    _switch_gpio.init (general_switch_gpio_dev);
    if (!_switch_gpio.isInit()) {
        syslog(LOG_ERR, "Unable to init output GPIO device %s", general_switch_gpio_dev);
    } else {
#ifdef SNOW_DEBUG
        syslog(LOG_DEBUG, "EjectionController: device %s GPIO inited", general_switch_gpio_dev);
#endif
    }
    return true;
}

void EjectionController::stop () {
    _direction_gpio.setValue (false);
    _rotation_gpio.setValue (false);
    _angle_pwm.setDutyCycle (0);
    _motor_pwm.setDutyCycle (0);
    _switch_gpio.setValue (false);
}

bool EjectionController::forceMotorSet (float value) {
    return _motor_pwm.setDutyCycle (value);
}

bool EjectionController::forceAngleSet (float value) {
    return _angle_pwm.setDutyCycle (value);
}

bool EjectionController::forceSetEnable (bool value) {
    return _switch_gpio.setValue (value);
}
