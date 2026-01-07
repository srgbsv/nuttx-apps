//
// Created by sergey on 22.10.24.
//
#include <errno.h>
#include <fcntl.h>
#include <nuttx/ioexpander/gpio.h>
#include <cstdio>
#include <string>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <syslog.h>
#include <unistd.h>

#include "providers/GpioOutput.hpp"
#include "Config.hpp"
#include "Logging.hpp"

GpioOutput::GpioOutput(const char * devpath)
{
    _devpath[0] = '\0';
    init (devpath);
}

bool GpioOutput::init(const char * devpath) {
    if (devpath) {
        strncpy(_devpath, devpath, sizeof(_devpath) - 1);
        _devpath[sizeof(_devpath) - 1] = '\0';
    } else {
        _devpath[0] = '\0';
    }
    _fd = open(devpath, O_WRONLY);
    if (_fd < 0) {
        snowerror("open %s failed: %s\n", devpath, strerror(errno));
        return false;
    }
    setValue (_default);
    snowinfo("GpioOutput: device %s opened successfully\n", devpath);
    _inited = true;
    return true;
}

bool GpioOutput::setValue(bool value) {
    int ret = ioctl(_fd, GPIOC_WRITE, (unsigned long)value);
    snowdebug("Setting GPIO %s to value %u\n", _devpath, (unsigned int)value);
    if (ret < 0)
    {
        snowerror("ERROR!!!!");
        int errcode = errno;
        fprintf(stderr,
        "ERROR: Failed to write value %u from %s: %d\n",
        (unsigned int)value, _devpath, errcode);
        close(_fd);
        return false;
    }
    return true;
}

GpioOutput::~GpioOutput() {
    close (_fd);
    _inited = false;
}

