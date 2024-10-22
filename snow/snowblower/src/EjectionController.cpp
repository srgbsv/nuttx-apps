//
// Created by sergey on 20.07.24.
//

#include "../include/EjectionController.h"
#include <fcntl.h>
#include <sys/ioctl.h>
#include <signal.h>

#include <nuttx/ioexpander/gpio.h>
#include <nuttx/config.h>

#define GPIOC_WRITE 1

EjectionController::EjectionController (
    const char * direction_gpio_dev,
    const char * rotation_gpio_dev,
    const char * motor_pwm_dev,
    const char * angle_gpio_dev
    )
    : _direction_gpio_path (direction_gpio_dev),
    _rotation_gpio_path (rotation_gpio_dev),
    _motor_pwm_path (motor_pwm_dev),
     _angle_gpio_path (angle_gpio_dev) {

}

/*bool EjectionController::init (const char* direction_gpio_dev, const char* rotation_gpio_dev, const char* motor_enable_gpio_dev, const char* angle_gpio_dev) {

}*/

EjectionController::EjectionController () {
}

void EjectionController::set_angle (float angle) {
    _target_angle = angle;
}

void EjectionController::set_rotation (float rotation) {
    _target_rotation = rotation;
}

void EjectionController::set_rotation_enable (bool run, bool clockwise){
    _rotation_enable = run;
    _rotation_direction = clockwise;
}

void EjectionController::set_motor_enable (bool enable) {
    _motor_enable = enable;
}

EjectionController::~EjectionController () {
    if (_rotation_gpio_fd) {
        close(_rotation_gpio_fd);
    }
    if (_direction_gpio_fd) {
        close(_direction_gpio_fd);
    }
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