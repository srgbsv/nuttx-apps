//
// Created by sergey on 20.07.24.
//

#ifndef APPS_BODY_CONTROLLER_H
#define APPS_BODY_CONTROLLER_H

#include "GpioOutput.h"
#include "config.h"

class BodyController {
    private:

    GpioOutput _lift_up_gpio;
    GpioOutput _lift_down_gpio;
    const int _event_count_thd = 4;

    int _lift_event_count = 0;
    int _lift_value = 0;

    public:

     bool init(
        const char* lift_up_gpio_dev,
        const char* lift_down_gpio_dev
    );

    void stop ();

    ~BodyController ();

    bool forceLiftSet (int lift);
    bool setLift (int lift);

    BodyController ();
};

#endif // APPS_BODY_CONTROLLER_H
