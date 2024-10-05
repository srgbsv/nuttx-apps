//
// Created by sergey.bosov on 05.10.2024.
//

#include "../include/PwmOutput.h"

PwmOutput::PwmOutput(const char * devpath)
{
    _devpath = devpath;
    _fd = open(_devpath, O_WRONLY);
}

PwmOutput::~PwmOutput()
{
    if (_fd) {
        close(_fd);
    }
}

PwmOutput::set_duty(float duty)
{
    pwminfo.set_duty(duty);
    update_pwm_state();
}

PwmOutput::update_pwm_state()
{
    ret = ioctl(fd, PWMIOC_SETCHARACTERISTICS,
              (unsigned long)((uintptr_t)&info));
    if (ret < 0)
    {
        printf("pwm_main: ioctl(PWMIOC_SETCHARACTERISTICS) failed: %d\n",
               errno);
    }
}

