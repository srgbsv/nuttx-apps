//
// Created by sergey on 20.07.24.
//

#ifndef APPS_MOTORCONTROLLER_H
#define APPS_MOTORCONTROLLER_H
#include "cstdio"
#include "string"

class MotorController {
    public:
    void set_enable(bool enable);
    MotorController(const char * _e_gpio_path);
    ~MotorController();

    private:
    std::string _e_gpio_path;
    int _e_gpio_fd;
    bool _motor_enable;
};

#endif // APPS_MOTORCONTROLLER_H
