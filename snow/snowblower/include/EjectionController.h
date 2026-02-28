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

    const int  _event_count_thd = 4;

    GpioOutput _rotation_gpio;
    GpioOutput _direction_gpio;
    GpioOutput _switch_gpio;
    PwmOutput _motor_pwm;
    PwmOutput _angle_pwm;

    float _current_rotation;
    float _target_rotation;
    int   _rotation_event_cnt = 0;
    float _current_angle;
    float _target_angle;
    int   _angle_current_cnt = 0;
    float _current_motor_value;
    int   _motor_value_cnt = 0;

    bool _rotation_enable;
    bool _rotation_direction;
    bool _angle_control_enable;
    bool _motor_enable;
    bool _current_switch_value;
    int  _switch_value_cnt = 0;

    public:

     bool init(
        const char* direction_gpio_dev,
        const char* rotation_gpio_dev,
        const char* motor_enable_gpio_dev,
        const char* angle_gpio_dev,
        const char* general_switch_gpio_dev
    );

    ~EjectionController();

    void setMotor(float motor_value);
    void setAngle(float angle);
    void setRotationEnable(bool run, bool clockwise);
    void setEnable(bool enable);

    bool forceMotorSet(float value);
    bool forceRotationSet(bool run, bool clockwise);
    bool forceAngleSet(float value);
    bool forceSetEnable(bool enable);

    void loop();
    void stop();

    EjectionController();
};

#endif // APPS_EJECTION_CONTROLLER_H
