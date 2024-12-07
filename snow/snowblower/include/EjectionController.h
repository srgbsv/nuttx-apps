//
// Created by sergey on 20.07.24.
//

#ifndef APPS_EJECTION_CONTROLLER_H
#define APPS_EJECTION_CONTROLLER_H

#include "GpioOutput.h"
#include "PwmOutput.h"
#include "config.h"
#include <cstdio>

class EjectionController {
    private:

    GpioOutput _rotation_gpio;
    GpioOutput _direction_gpio;
    PwmOutput _motor_pwm;
    PwmOutput _angle_pwm;

    float _current_rotation;
    float _target_rotation;
    float _current_angle;
    float _target_angle;
    bool _rotation_enable;
    bool _rotation_direction;
    bool _angle_control_enable;
    bool _motor_enable;

    public:

     bool init(
        const char* direction_gpio_dev,
        const char* rotation_gpio_dev,
        const char* motor_enable_gpio_dev,
        const char* angle_gpio_dev
    );

    ~EjectionController();

    void setMotorEnable(bool enable);
    void setAngle(float angle);
    void setRotation(float rotation);
    void setRotationEnable(bool run, bool clockwise);

    bool forceMotorSet(float value);
    bool forceRotationSet(bool run, bool clockwise);
    bool forceAngleSet(float value);

    void loop();
    void stop();

    EjectionController();
};

#endif // APPS_EJECTION_CONTROLLER_H
