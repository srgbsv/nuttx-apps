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

#define GPIOC_WRITE 1

/*bool EjectionController::init (const char* direction_gpio_dev, const char* rotation_gpio_dev, const char* motor_enable_gpio_dev, const char* angle_gpio_dev) {

}*/

EjectionController::EjectionController () {
}

void EjectionController::setAngle (float angle) {
    _target_angle = angle;
}

void EjectionController::setRotation (float rotation) {
    _target_rotation = rotation;
}

void EjectionController::setRotationEnable (bool run, bool clockwise){
    _rotation_enable = run;
    _rotation_direction = clockwise;
}

void EjectionController::setMotorEnable (bool enable) {
    _motor_enable = enable;
}

EjectionController::~EjectionController () {

}
void EjectionController::loop () {
    int ret = 0;
    int outvalue = 0;
    int errcode = errno;
    /*if (_motor_enable_gpio_fd) {
        if (_motor_enable) {
            outvalue = 1;
        } else {
            outvalue = 0;
        }
        ret = ioctl(_motor_enable_gpio_fd, GPIOC_WRITE, outvalue);
        if (ret < 0)
        {
            errcode = errno;
            fprintf(stderr,
            "ERROR: Failed to write value %u from %s: %d\n",
                    (unsigned int)outvalue, _motor_enable_gpio_path.c_str(), errcode);
        }
    }
    if (_direction_gpio_fd) {
        if (_rotation_direction) {
            outvalue = 1;
        } else {
            outvalue = 0;
        }
        ret = ioctl(_direction_gpio_fd, GPIOC_WRITE, outvalue);
        if (ret < 0)
        {
            errcode = errno;
            fprintf(stderr,
            "ERROR: Failed to write value %u from %s: %d\n",
            (unsigned int)outvalue, _direction_gpio_path.c_str(), errcode);
        }
    }
    if (_motor_enable_gpio_fd) {
        if (_rotation_enable) {
            outvalue = 1;
        } else {
            outvalue = 0;
        }
        ret = ioctl(_rotation_gpio_fd, GPIOC_WRITE, outvalue);
        if (ret < 0)
        {
            errcode = errno;
            fprintf(stderr,
            "ERROR: Failed to write value %u from %s: %d\n",
            (unsigned int)outvalue, _rotation_gpio_path.c_str(), errcode);
        }
    } */
}
bool EjectionController::init (const char* direction_gpio_dev,
    const char* rotation_gpio_dev,
    const char* motor_enable_gpio_dev,
    const char* angle_gpio_dev
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
    return true;
}

void EjectionController::stop () {
    _direction_gpio.setValue (false);
    _rotation_gpio.setValue (false);
    _angle_pwm.setDutyCycle (0);
    _motor_pwm.setDutyCycle (0);
}

bool EjectionController::forceMotorSet (float value) {
    return _motor_pwm.setDutyCycle (value);
}

bool EjectionController::forceRotationSet (bool run, bool clockwise) {
    if (_direction_gpio.setValue (clockwise) && _rotation_gpio.setValue (run)) {
        return true;
    }

    return false;
}

bool EjectionController::forceAngleSet (float value) {
    return _angle_pwm.setDutyCycle (value);
}
