#pragma once

#include "Config.hpp"
#include <memory>
#include "state/State.hpp"
#include "providers/GpioOutput.hpp"
#include "providers/PwmOutput.hpp"
#include "providers/RotationProvider.hpp"

class EjectionController {
    private:

    const int _event_count_thd = 4;

    GpioOutput       _switch_gpio;
    PwmOutput        _motor_pwm;
    PwmOutput        _angle_pwm;
    GpioOutput       _rotation_gpio;
    GpioOutput       _direction_gpio;
    RotationProvider _rotation_provider;

    float            _current_rotation;
    float            _target_rotation;
    int              _rotation_event_cnt = 0;

    float            _current_angle;
    float            _target_angle;
    int              _angle_current_cnt = 0;

    float            _current_motor_value;
    int              _motor_value_cnt = 0;

    bool             _rotation_enable;
    bool             _rotation_direction;
    bool             _angle_control_enable;
    bool             _motor_enable;
    bool             _current_switch_value;
    int              _switch_value_cnt = 0;

    std::shared_ptr<State> _state;

    public:

     bool init(
        std::shared_ptr<State> state,
        const char* testing_switch_dev,
        const char* rotation_first_sensor_dev,
        const char* rotation_second_sensor_dev,
        const char* sensor_first_enable_gpio_dev,
        const char* rotation_encoder_dev,
        const char* rotation_enable_gpio_dev,
        const char* rotation_direction_gpio_dev,
        const char* motor_pwm_dev,
        const char* angle_pwm_dev
    );
    
    EjectionController();
    ~EjectionController();

    void setMotor(float motor_value);
    void setAngle(float angle);
    void setRotationAngel(float angle);
    void setEnable(bool enable);

    bool forceMotorSet(float value);
    bool forceAngleSet(float value);

    void loop();
    void stop();

};

