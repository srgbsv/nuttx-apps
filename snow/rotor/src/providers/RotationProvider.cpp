#include "providers/RotationProvider.hpp"
#include "Logging.hpp"
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

void RotationProvider::init (
    const char* first_sensor_gpio_devpath,
    const char* second_sensor_gpio_devpath,
    const char* reset_btn_gpio_devpath,
    const char* rotation_enable_gpio_devpath,
    const char* direction_gpio_devpath,
    const char* encoder_devpath
){
    _first_sensor_gpio.init(first_sensor_gpio_devpath);
    _second_sensor_gpio.init(second_sensor_gpio_devpath);
    _reset_btn.init(reset_btn_gpio_devpath);
    _rotation_enable_gpio.init(rotation_enable_gpio_devpath);
    _direction_gpio.init(direction_gpio_devpath);
    _encoder_input.init(encoder_devpath);
}


float RotationProvider::getCurrentAngle () {
    return _current_angle;
}

void RotationProvider::setTargetValue (float angle) {
    _target_angle = angle;
}


void RotationProvider::checkAndUpdate () {
    updateAngle();
    if (std::fabs(_target_angle - _current_angle) < _comparison_thd) {
        // rotation not needed
        _direction_gpio.setValue(0);
        _rotation_enable_gpio.setValue(0);
    } else if (_first_sensor_gpio.getState() == 0 || _second_sensor_gpio.getState() == 0) {
        if (_first_sensor_gpio.getState() == 0) {
            snowerror("First sensor triggered. Error");
        } else {
            snowerror("Second sensor triggered. Error");
        }
        snowdebug("Current angle: %f, _target angle: %f", _current_angle, _target_angle);
        _direction_gpio.setValue(0);
        _rotation_enable_gpio.setValue(0);
    } else {
        if (_target_angle - _current_angle > _comparison_thd) {
            _direction_gpio.setValue(1);
        } else {
            _direction_gpio.setValue(0);
        }
        _rotation_enable_gpio.setValue(1);
    }
}

void RotationProvider:: updateAngle() {
    int encoder_state = _encoder_input.getState();
    _current_angle = (float) encoder_state / _impulses_per_degree;
}

void RotationProvider::stop() {
    _rotation_enable_gpio.setValue(0);
}

RotationProvider::~RotationProvider () {
}

void RotationProvider::calibrate() {
    _total_impulse_count = 0;
    if (_first_sensor_gpio.getState() == 0) {
        _direction_gpio.setValue(0); 
        _rotation_enable_gpio.setValue(0);
    }
   
    
}

void RotationProvider::gotoZeroPosition() {
    _direction_gpio.setValue(0); 
    _rotation_enable_gpio.setValue(0);
    if (_first_sensor_gpio.getState() == 0) {
        snowdebug("Already at zero position");
        return;
    }
    _direction_gpio.setValue(0); 
    _rotation_enable_gpio.setValue(1);
    while (_first_sensor_gpio.getState() != 0) {
        updateAngle();
        usleep(1000);
    }
}

void RotationProvider::forceRotate(bool enable, bool direction) {
    _direction_gpio.setValue(direction);
    _rotation_enable_gpio.setValue(!enable);
}

