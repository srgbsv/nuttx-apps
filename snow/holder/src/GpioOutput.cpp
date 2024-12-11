//
// Created by sergey on 22.10.24.
//
#include "../include/GpioOutput.h"
#include "../include/config.h"
#include <errno.h>
#include <fcntl.h>
#include <nuttx/ioexpander/gpio.h>
#include <stdio.h>
#include <string>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <syslog.h>
#include <unistd.h>

bool GpioOutput::init(const char * devpath) {
    _devpath = std::string(devpath);
    _fd = open(devpath, O_WRONLY);
    if (_fd < 0) {
        syslog(LOG_ERR, "open %s failed: %s", devpath, strerror(errno));
        return false;
    }
    setValue (_default);
    syslog(LOG_INFO, "GpioOutput: device %s opened successfully", devpath);
    _inited = true;
    return true;
}

bool GpioOutput::setValue(bool value) {
    int ret = ioctl(_fd, GPIOC_WRITE, (unsigned long)value);
    if (ret < 0)
    {
        syslog(LOG_ERR, "ERROR!!!!");
        int errcode = errno;
        fprintf(stderr,
        "ERROR: Failed to write value %u from %s: %d\n",
        (unsigned int)value, _devpath.c_str(), errcode);
        close(_fd);
        return false;
    }
    return true;
}

GpioOutput::~GpioOutput() {
    close (_fd);
    _inited = false;
}

