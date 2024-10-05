//
// Created by sergey on 20.07.24.
//

#ifndef APPS_EJECTION_CONTROLLER_H
#define APPS_EJECTION_CONTROLLER_H

#include "cstdio"
#include "string"
class EjectionController {
    public:

    EjectionController (
        const char* direction_gpio_dev,
        const char* rotation_gpio_dev,
        const char* motor_enable_gpio_dev
    );

    ~EjectionController();

    bool set_motor_enable(bool enable);
    bool set_angle(float angle);
    bool set_rotation(float rotation);
    void set_rotation_enable(bool run, bool clockwise);
    void loop();
    EjectionController();

    private:

    std::string _e_gpio_path;
    int _e_gpio_fd = 0;
    std::string _direction_gpio_path;
    int _direction_gpio_fd = 0;
    std::string _rotation_gpio_path;
    int _rotation_gpio_fd = 0;
    std::string _motor_enable_gpio_path;
    int _motor_enable_gpio_fd = 0;

    float _current_rotation;
    float _target_rotation;
    float _current_angle;
    float _target_angle;
    bool _rotation_enable;
    bool _rotation_direction;
    bool _angle_control_enable;
    bool _motor_enable;

    int task_spawn (int argc, char** argv);
};

#endif // APPS_EJECTION_CONTROLLER_H
