//
// Created by sergey on 20.07.24.
//

#include "../include/MotorController.h"
#include <fcntl.h>

MotorController::MotorController(const char * e_gpio_path) {
    _e_gpio_path = e_gpio_path;
    _e_gpio_fd = open(_e_gpio_path.c_str(), O_RDWR);
}

MotorController::~MotorController() {
    if (_e_gpio_fd) {
        close (_e_gpio_fd);
    }
}

void MotorController::set_enable (bool enable) {
    _motor_enable = enable;
}