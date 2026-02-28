//
// Created by sergey on 20.07.24.
//

#include "BodyController.h"
#include "config.h"
#include <fcntl.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <syslog.h>

#include <nuttx/ioexpander/gpio.h>
#include <nuttx/config.h>

#define GPIOC_WRITE 1

/*bool EjectionController::init (const char* direction_gpio_dev, const char* rotation_gpio_dev, const char* motor_enable_gpio_dev, const char* angle_gpio_dev) {

}*/

BodyController::BodyController () {
}

bool BodyController::init (
    const char* lift_up_gpio_dev,
    const char* lift_down_gpio_dev
) {
    _lift_up_gpio.init (lift_up_gpio_dev);
    if (!_lift_up_gpio.isInit()) {
        syslog(LOG_ERR, "Unable to init output GPIO device %s", lift_up_gpio_dev);
    } else {
#ifdef HOLDER_DEBUG
        syslog(LOG_DEBUG, "BodyController: device %s GPIO inited", lift_up_gpio_dev);
#endif
    }
    _lift_down_gpio.init (lift_down_gpio_dev);
    if (!_lift_down_gpio.isInit()) {
        syslog(LOG_ERR, "Unable to init output GPIO device %s", lift_down_gpio_dev);
    } else {
#ifdef HOLDER_DEBUG
        syslog(LOG_DEBUG, "BodyController: device %s GPIO inited", lift_down_gpio_dev);
#endif
    }
    return true;
}

void BodyController::stop () {
    _lift_down_gpio.setValue (false);
    _lift_up_gpio.setValue (false);
}

bool BodyController::setLift (int lift) {
    if (lift != _lift_value) {
        _lift_event_count = 0;
        _lift_value = lift;
        syslog(LOG_DEBUG, "BodyController: changing lift value: %d -> %d ", _lift_value, lift);
    } else {
        _lift_event_count++;
        if (_lift_event_count > _event_count_thd) {
            return forceLiftSet(lift);
        }
    }

    return false;
}

bool BodyController::forceLiftSet (int lift) {
    if (_lift_up_gpio.isInit() && _lift_down_gpio.isInit()) {
        _lift_up_gpio.setValue(lift > 0 ? false : true);
        _lift_down_gpio.setValue(lift < 0 ? false : true);
        return true;
    }
    return false;
}

BodyController::~BodyController() {
    stop();
}
