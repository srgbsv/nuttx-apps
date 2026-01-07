#pragma once

#include <stdio.h>

#include <nuttx/config.h>
#include <nuttx/timers/pwm.h>

class PwmOutput {
    private:
    // fixed-size buffer for embedded builds (no std::string)
    char    _devpath[16];

    int _fd = -1;
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