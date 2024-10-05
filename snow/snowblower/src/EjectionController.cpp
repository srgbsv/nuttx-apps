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
    const char * e_gpio_dev,
    const char * direction_gpio_dev,
    const char * rotation_gpio_dev,
    const char * motor_enable_gpio_dev
    const char* angle_gpio_dev
    )
    : _e_gpio_path(e_gpio_dev),
    _direction_gpio_path (direction_gpio_dev),
    _rotation_gpio_path (rotation_gpio_dev),
    _motor_enable_gpio_path (motor_enable_gpio_dev),
     _angle_gpio_path (angle_gpio_dev) {
    _e_gpio_fd = open(e_gpio_dev, O_RDWR);
    _direction_gpio_fd = open (direction_gpio_dev, O_RDWR);
    _rotation_gpio_fd = open (rotation_gpio_dev, O_RDWR);
    _motor_enable_gpio_fd = open (motor_enable_gpio_dev, O_RDWR);
}

int EjectionController::task_spawn (int argc, char** argv) {
    return 0;
}

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
    if (_e_gpio_fd) {
        close(_e_gpio_fd);
    }
    if (_rotation_gpio_fd) {
        close(_rotation_gpio_fd)
    }
    if (_direction_gpio_fd) {
        close(_direction_gpio_fd);
    }
    if (_motor_fd) {
        close(_motor_fd);
    }
    if (_angle_gpio_fd) {
        close(_angle_gpio_fd);
    }
}
void EjectionController::loop () {
    int ret = 0;
    int outvalue = 0;
    int errcode = errno;
    if (_motor_enable_gpio_fd) {
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
    if (_e_gpio_fd) {
        if (_enable_rotation) {
            outvalue = 1;
        } else {
            outvalue = 0;
        }
        ret = ioctl(_e_gpio_fd, GPIOC_WRITE, outvalue);
        if (ret < 0)
        {
            errcode = errno;
            fprintf(stderr,
            "ERROR: Failed to write value %u from %s: %d\n",
                    (unsigned int)outvalue, _e_gpio_path.c_str(), errcode);
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

}
