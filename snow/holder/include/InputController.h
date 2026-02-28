//
// Created by sergey on 11.10.24.
//

#ifndef HOLDER_INPUTCONTROLLER_H
#define HOLDER_INPUTCONTROLLER_H
#include "PwmInput.h"
#include "GpioInput.h"

class InputController {
    private:
    PwmInput    _pwm_input_lift;

    public:
    
    int getLiftValue();
    int getLiftValueRaw();
    InputController (const char* lift_enable_dev);
    bool init (const char* lift_enable_dev);
    InputController() {}
};


#endif // HOLDER_INPUTCONTROLLER_H
