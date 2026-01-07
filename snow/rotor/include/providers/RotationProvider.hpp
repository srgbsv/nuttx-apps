#pragma once

#include "providers/GpioInput.hpp"
#include "providers/GpioOutput.hpp"
#include "providers/EncoderProvider.hpp"

class RotationProvider
{
private:
    const int   _full_circle         = 300; // TODO check real angle. Angle between extreme sensor triggers
    const float _comparison_thd      = 1.0f; // angle comparison difference threshold
    const int   ERROR_FIRST_SENSOR   = 1;
    const int   ERROR_SECOND_SENSOR  = 2;

    int         _total_impulse_count = 6000; // TODO check
    float       _impulses_per_degree = (float)_total_impulse_count / (float)_full_circle;
    int         _impulse_count       = 0; //
    float       _target_angle        = 0; //
    float       _current_angle       = 0; //
    int         _last_encoder_state  = 0; //
    int         _is_sensor_triggered = 0; // -1 for first sensor, 1 for second sensor, 0 for none

    bool        _error               = false;
    int         _error_code          = 0;

    /* data */
    GpioInput           _first_sensor_gpio;
    GpioInput           _second_sensor_gpio;
    GpioInput           _reset_btn;

    GpioOutput          _rotation_enable_gpio;
    GpioOutput          _direction_gpio;

    EncoderProvider     _encoder_input;
    
    void updateAngle ();
    void gotoZeroPosition ();

public:

    void  calibrate();

    float getCurrentAngle();
    void  setTargetValue(float angle);

    void  checkAndUpdate();
    void  stop();
    void  forceRotate(bool enable, bool direction);

    void init (
        const char* first_sensor_gpio_devpath,
        const char* second_sensor_gpio_devpath,
        const char* reset_btn_gpio_devpath,
        const char* rotation_enable_gpio_devpath,
        const char* direction_gpio_devpath,
        const char* encoder_devpath
    );
    ~RotationProvider();
};
