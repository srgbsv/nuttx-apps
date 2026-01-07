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

    GpioInput        _switch_gpio;
    PwmOutput        _motor_pwm;
    PwmOutput        _angle_pwm;
    GpioOutput       _rotation_gpio;
    GpioOutput       _direction_gpio;
    RotationProvider _rotation_provider;

    /**
     * Current rotation angle
     */
    float            _current_rotation;
    /**
     * Target rotation angle
     */
    float            _target_rotation;

    /**
     * Current blowing angle. Servo
     */
    float            _current_angle;
    /**
     * Target blowing angle. Servo
     */
    float            _target_angle;


    float            _current_motor_value;

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
    void setEnable(bool enable);

    bool forceMotorSet(float value);
    bool forceAngleSet(float value);
    void forceRotate(bool enable, bool direction);

    void loop();
    void stop();

};

