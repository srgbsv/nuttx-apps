//
// Created by sergey.bosov on 05.10.2024.
//

#include "../include/PwmOutput.h"

#include <sys/types.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <syslog.h>

PwmOutput::PwmOutput(const char * devpath)
{
    init (devpath);
}

bool PwmOutput::init (const char* devpath) {
    _devpath = std::string(devpath);
    _fd = open (devpath, O_RDWR);
    if (_fd < 0) {
        syslog(LOG_ERR, "open %s failed: %s", devpath, strerror(errno));
        return false;
    }
    _inited = true;

    return true;
}

PwmOutput::~PwmOutput()
{
    if (_fd) {
        close(_fd);
    }
}

int PwmOutput::setDutyCycle (float duty)
{
    _pwminfo.duty = duty;
    updatePwmState();
}

bool PwmOutput::updatePwmState()
{
    int ret = ioctl(_fd, PWMIOC_SETCHARACTERISTICS,
              (unsigned long)((uintptr_t)&_pwminfo));
    if (ret < 0)
    {
        printf("pwm_main: ioctl(PWMIOC_SETCHARACTERISTICS) failed: %d\n",
               errno);
    }
    return true;
}

