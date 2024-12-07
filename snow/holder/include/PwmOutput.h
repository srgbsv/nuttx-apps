//
// Created by sergey on 01.09.24.
//

#ifndef HOLDER_PWMOUTPUT_H
#define HOLDER_PWMOUTPUT_H

#include "cstdlib"
#include "dspb16.h"
#include "string"
#include <nuttx/config.h>
#include <nuttx/timers/pwm.h>

class PwmOutput {
    private:
    std::string     _devpath;

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


#endif // HOLDER_PWMOUTPUT_H
