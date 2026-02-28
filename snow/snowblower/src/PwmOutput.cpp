//
// Created by sergey.bosov on 05.10.2024.
//

#include "../include/PwmOutput.h"

#include <sys/types.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <syslog.h>
#include "config.h"

PwmOutput::PwmOutput(const char * devpath)
{
    init (devpath);
}

bool PwmOutput::init (const char* devpath) {
    _pwminfo.frequency = 50;
    _pwminfo.duty = 0;
    _devpath = std::string(devpath);
    _fd = open (devpath, O_RDONLY);
    if (_fd < 0) {
        syslog(LOG_ERR, "open %s failed: %s", devpath, strerror(errno));
        return false;
    }
    syslog(LOG_INFO, "PwmOutput: device %s opened successfully", devpath);
    _inited = true;

    return true;
}

PwmOutput::~PwmOutput()
{
    if (_fd) {
        close(_fd);
    }
}

bool PwmOutput::setDutyCycle (uint8_t duty)
{
#ifdef SNOW_DEBUG
    syslog(LOG_INFO, "Set duty for device %s: %d", _devpath.c_str(), duty);
#endif
    _pwminfo.duty = duty ? \
    b16divi(uitoub16(duty) - 1, 100) : 0;
    return updatePwmState();
}

bool PwmOutput::updatePwmState()
{
#ifdef SNOW_DEBUG
    syslog(LOG_INFO, "Start pulse training for device %s: freq %lu Hz, duty: %lu ", _devpath.c_str(), _pwminfo.frequency, _pwminfo.duty);
#endif
    int ret = ioctl(_fd, PWMIOC_SETCHARACTERISTICS,
              (unsigned long)((uintptr_t)&_pwminfo));
    if (ret < 0)
    {
        syslog(LOG_ERR, "PwmOutput: ioctl(PWMIOC_SETCHARACTERISTICS) failed: %d\n",
               errno);
        return false;
    }
    ret = ioctl(_fd, PWMIOC_START, 0);
    if (ret < 0)
    {
        syslog(LOG_ERR, "PwmOutput: ioctl(PWMIOC_START) failed: %d\n", errno);
        return false;
    }
    return true;
}

