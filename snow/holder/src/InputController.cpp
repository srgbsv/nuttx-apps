//
// Created by sergey on 11.10.24.
//

#include "../include/InputController.h"
#include "syslog.h"
#include "../include/config.h"

bool InputController::init(
    const char* lift_enable_dev
) {
    _pwm_input_lift.init(lift_enable_dev, 4, 9);
    if (!_pwm_input_lift.isInit ()) {
        syslog(LOG_ERR, "Can't input pwm device %s", lift_enable_dev);
    } else {
#ifdef HOLDER_DEBUG
        syslog(LOG_DEBUG, "InputController: device %s PWM inited", lift_enable_dev);
#endif
    }

    return true;
}

int InputController::getLiftValue() {
    auto lift_pwm = _pwm_input_lift.getDutyCycleNormalized();
#ifdef HOLDER_DEBUG
    syslog(LOG_DEBUG, "New duty cycle: %d", lift_pwm);
#endif    
    if (lift_pwm > 65) return 1;
    if (lift_pwm < 45) return -1;
    return 0;
}

int InputController::getLiftValueRaw () {
    return _pwm_input_lift.getDutyCycle();
}
