//
// Created by sergey on 11.10.24.
//

#ifndef SNOWBLOWER_INPUTCONTROLLER_H
#define SNOWBLOWER_INPUTCONTROLLER_H
#include "PwmInput.h"
#include "GpioInput.h"

class InputController {
    private:
    PwmInput    _pwm_input_motor;
    PwmInput    _pwm_input_rotation;
    PwmInput    _pwm_input_ejection;
    GpioInput   _gpio_in_test_btn;

    public:
    InputController(
        const char * _motor_enable_dev,
        const char * _rotation_dev,
        const char * _ejection_dev,
        const char * _test_btn_dev
    );

    int getMotorValue();
    int getMotorValueRaw();
    int getRotationValue();
    int getRotationValueRaw();
    int getEjectionValue();
    int getEjectionValueRaw();
    bool isTestBtnPressed();
    bool init (const char* _motor_enable_dev,
    const char* _rotation_dev,
    const char* _ejection_dev,
    const char* _test_btn_dev);
    InputController() {}
};


#endif // SNOWBLOWER_INPUTCONTROLLER_H
