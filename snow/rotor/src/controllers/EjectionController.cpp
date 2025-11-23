//
// Created by sergey on 20.07.24.
//

#include <fcntl.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <syslog.h>

#include <nuttx/ioexpander/gpio.h>
#include <nuttx/config.h>
#include "controllers/EjectionController.hpp"
#include "Logging.hpp"

EjectionController::EjectionController() {

};

bool EjectionController::init (
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
) {

    _state = state;

    // Initialize rotation provider which handles sensors, encoder and control gpios
    _rotation_provider.init(
        rotation_first_sensor_dev,
        rotation_second_sensor_dev,
        testing_switch_dev,
        rotation_enable_gpio_dev,
        rotation_direction_gpio_dev,
        rotation_encoder_dev
    );

    // Also keep direct gpio handles for quick control if needed
    _rotation_gpio.init (rotation_enable_gpio_dev);
    if (!_rotation_gpio.isInit()) {
        snowerror("Unable to init rotation enable GPIO device %s", rotation_enable_gpio_dev);
    }

    _direction_gpio.init (rotation_direction_gpio_dev);
    if (!_direction_gpio.isInit()) {
        snowerror("Unable to init direction GPIO device %s", rotation_direction_gpio_dev);
    }

    // Initialize motor and angle PWMs
    _motor_pwm.init (motor_pwm_dev);
    if (!_motor_pwm.isInit()) {
        snowerror("Unable to init output PWM device %s", motor_pwm_dev);
    }

    _angle_pwm.init (angle_pwm_dev);
    if (!_angle_pwm.isInit()) {
        snowerror("Unable to init output PWM device %s", angle_pwm_dev);
    }

    // Initialize testing/general switch
    _switch_gpio.init (testing_switch_dev);
    if (!_switch_gpio.isInit()) {
        snowerror("Unable to init switch GPIO device %s", testing_switch_dev);
    }

    return true;

}


EjectionController::~EjectionController () {

}

void EjectionController::stop () {
    _rotation_provider.stop();
    _angle_pwm.setDutyCycle (0);
    _motor_pwm.setDutyCycle (0);
    _switch_gpio.setValue (false);
}

bool EjectionController::forceMotorSet (float value) {
    return _motor_pwm.setDutyCycle (value);
}

bool EjectionController::forceAngleSet (float value) {
    return _angle_pwm.setDutyCycle (value);
}

void EjectionController::loop () {
    _rotation_provider.checkAndUpdate();
}

void EjectionController::setRotationAngel (float angle) {
    _rotation_provider.setTargetValue(angle);
}