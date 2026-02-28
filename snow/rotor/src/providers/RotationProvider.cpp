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
    _rotation_enable_gpio.setValue(1);
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
    auto first_sensor_state = _first_sensor_gpio.getState();
    auto second_sensor_state = _second_sensor_gpio.getState();

    if (_first_sensor_gpio.getState() == 0 || _second_sensor_gpio.getState() == 0) {
        if (_first_sensor_gpio.getState() == 0) {
            snowerror("First sensor triggered. Error");
        } else {
            snowerror("Second sensor triggered. Error");
        }
        //snowdebug("Current angle: %f, _target angle: %f", _current_angle, _target_angle);
        //_direction_gpio.setValue(0);
        //_rotation_enable_gpio.setValue(0);
    } else if (std::fabs(_target_angle - _current_angle) < _comparison_thd) {
        // rotation not needed
        //_direction_gpio.setValue(0);
        //_rotation_enable_gpio.setValue(0);
    } else {
        //if (_target_angle - _current_angle > _comparison_thd) {
        //    _direction_gpio.setValue(1);
        //} else {
        //    _direction_gpio.setValue(0);
        //}
        //_rotation_enable_gpio.setValue(1);
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
    snowinfo("Starting calibration...");

    // Вращаем к первому датчику (положение 0)
    snowinfo("Rotating to the first sensor (zero position)...");
    _direction_gpio.setValue(1); // Предполагаем, что 1 - это направление к первому датчику
    _rotation_enable_gpio.setValue(0); // Предполагаем, что 0 - это включение вращения

    while (_first_sensor_gpio.getState() != 0) {
        usleep(10000); // 10ms
    }
    _rotation_enable_gpio.setValue(1); // Выключаем вращение
    snowinfo("First sensor reached.");

    // Сбрасываем энкодер
    _encoder_input.resetCounter();
    snowinfo("Encoder reset.");

    // Вращаем ко второму датчику
    snowinfo("Rotating to the second sensor...");
    _direction_gpio.setValue(0); // Вращаем в обратную сторону
    _rotation_enable_gpio.setValue(0); // Включаем вращение

    while (_second_sensor_gpio.getState() != 0) {
        usleep(10000); // 10ms
    }
    _rotation_enable_gpio.setValue(1); // Выключаем вращение
    snowinfo("Second sensor reached.");

    // Сохраняем значение энкодера и вычисляем импульсы на градус
    _total_impulse_count = _encoder_input.getState();
    _impulses_per_degree = (float)_total_impulse_count / 180.0f; // Предполагаем, что между датчиками 180 градусов

    snowinfo("Calibration finished. Total impulses: %d, impulses per degree: %f", _total_impulse_count, _impulses_per_degree);
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
