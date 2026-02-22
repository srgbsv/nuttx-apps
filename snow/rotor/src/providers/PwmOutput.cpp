//
// Created by sergey.bosov on 05.10.2024.
//
#include <sys/types.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <syslog.h>
#include <string.h>

#include "providers/PwmOutput.hpp"
#include "Logging.hpp"

PwmOutput::PwmOutput(const char * devpath)
{
    _devpath[0] = '\0';
    init (devpath);
}

bool PwmOutput::init (const char* devpath) {
    _pwminfo.frequency = 50;
    _pwminfo.duty = 0;
    if (devpath) {
        strncpy(_devpath, devpath, sizeof(_devpath) - 1);
        _devpath[sizeof(_devpath) - 1] = '\0';
    } else {
        _devpath[0] = '\0';
    }
    _fd = open (_devpath, O_RDONLY);
    if (_fd < 0) {
        snowerror("open %s failed: %s", _devpath, strerror(errno));
        _fd = -1;
        return false;
    }
    snowinfo("PwmOutput: device %s opened successfully\n", _devpath);
    _inited = true;

    return true;
}

PwmOutput::~PwmOutput()
{ 
    if (_fd >= 0) {
        close(_fd);
        _fd = -1;
    }
}

//Constructor copy

bool PwmOutput::setDutyCycle (uint8_t duty)
{
    snowdebug("Set duty for device %s: %d", _devpath, duty);
    _pwminfo.duty = b16divi(uitoub16(duty), 100);
    return updatePwmState();
}

bool PwmOutput::updatePwmState()
{
    snowdebug("Start pulse training for device %s: freq %lu Hz, duty: %lu \n", _devpath, _pwminfo.frequency, _pwminfo.duty);
    int ret = ioctl(_fd, PWMIOC_SETCHARACTERISTICS,
              (unsigned long)((uintptr_t)&_pwminfo));
    if (ret < 0)
    {
        snowerror("PwmOutput: ioctl(PWMIOC_SETCHARACTERISTICS) failed: %d\n",
               errno);
        return false;
    }
    ret = ioctl(_fd, PWMIOC_START, 0);
    if (ret < 0)
    {
        snowerror("PwmOutput: ioctl(PWMIOC_START) failed: %d\n", errno);
        return false;
    }
    return true;
}
