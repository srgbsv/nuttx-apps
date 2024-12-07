//
// Created by sergey on 20.07.24.
//

#ifndef APPS_EJECTION_CONTROLLER_H
#define APPS_EJECTION_CONTROLLER_H

#include "GpioOutput.h"
#include "PwmOutput.h"
#include "config.h"
#include <cstdio>

class BodyController {
    private:

    GpioOutput _lift_up_gpio;
    GpioOutput _lift_down_gpio;

    public:

     bool init(
        const char* lift_up_gpio_dev,
        const char* lift_down_gpio_dev
    );

    void stop ();

    ~BodyController ();

    bool forceLiftSet (int lift);

    BodyController ();
};

#endif // APPS_EJECTION_CONTROLLER_H
