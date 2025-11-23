#pragma once

#include <stdio.h>
#include <string>

#include <nuttx/config.h>
#include <nuttx/timers/pwm.h>

class PwmOutput {
    private:
    char *  _devpath;

    int _fd;
    pwm_info_s _pwminfo;
    bool _inited = false;

    public:
    PwmOutput(const char * devpath);
    ~PwmOutput();

    bool init(const char * devpath);
    bool setDutyCycle(uint8_t);
    bool  updatePwmState();
    PwmOutput() {};

    [[nodiscard]] bool isInit () const {
        return _inited;
    }
};