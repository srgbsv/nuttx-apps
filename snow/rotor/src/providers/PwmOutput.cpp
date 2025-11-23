//
// Created by sergey.bosov on 05.10.2024.
//
#include <sys/types.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <syslog.h>

#include "providers/PwmOutput.hpp"
#include "Logging.hpp"

PwmOutput::PwmOutput(const char * devpath)
{
    init (devpath);
}

bool PwmOutput::init (const char* devpath) {
    _pwminfo.frequency = 50;
    _pwminfo.duty = 0;
    strcpy(_devpath, devpath);
    _fd = open (devpath, O_RDONLY);
    if (_fd < 0) {
        snowerror("open %s failed: %s", devpath, strerror(errno));
        return false;
    }
    snowinfo("PwmOutput: device %s opened successfully", devpath);
    _inited = true;

    return true;
}

PwmOutput::~PwmOutput()
{ 
    if (_fd) {
        close(_fd);
    }
}

//Constructor copy

bool PwmOutput::setDutyCycle (uint8_t duty)
{
#ifdef HOLDER_DEBUG
    snowinfo("Set duty for device %s: %d", _devpath, duty);
#endif
    _pwminfo.duty = duty ? \
    b16divi(uitoub16(duty) - 1, 100) : 0;
    return updatePwmState();
}

bool PwmOutput::updatePwmState()
{
    snowdebug("Start pulse training for device %s: freq %lu Hz, duty: %lu ", _devpath, _pwminfo.frequency, _pwminfo.duty);
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

