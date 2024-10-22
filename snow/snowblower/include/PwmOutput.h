//
// Created by sergey on 01.09.24.
//

#ifndef SNOWBLOWER_PWMOUTPUT_H
#define SNOWBLOWER_PWMOUTPUT_H

#include "cstdlib"
#include "dspb16.h"
#include "string"
#include <nuttx/config.h>
#include <nuttx/timers/pwm.h>

class PwmOutput {
    public:
    PwmOutput(const char * devpath);
    ~PwmOutput();

    bool init(const char * devpath);
    int setDutyCycle(float);
    bool  updatePwmState();
    PwmOutput() {};

    private:
    std::string     _devpath;

    int _fd;
    pwm_info_s _pwminfo;
    bool _inited = false;
};


#endif // SNOWBLOWER_PWMOUTPUT_H
