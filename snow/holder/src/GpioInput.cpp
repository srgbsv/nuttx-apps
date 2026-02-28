//
// Created by sergey on 10.10.24.
//

#include "../include/GpioInput.h"
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

GpioInput::GpioInput (const char* devpath) {
    init (devpath);
}

GpioInput::~GpioInput () {
    close(_fd);
}
bool GpioInput::init (const char* devpath) {
    _devpath = std::string(devpath);
    _fd = open(devpath, O_RDONLY);
    if (_fd < 0) {
        syslog(LOG_ERR, "open %s failed: %s", devpath, strerror(errno));
        return false;
    }
    syslog(LOG_INFO, "GpioInput: device %s opened successfully", devpath);
    _inited = true;
    return true;
}

int GpioInput::getState () {
    bool invalue = false;

    int ret = ioctl(_fd, GPIOC_READ, (unsigned long)((uintptr_t)&invalue));
    if (ret < 0)
    {
        int errcode = errno;
        syslog(LOG_ERR, "ERROR: Failed to read value from %s: %d\n",
        _devpath.c_str(), errcode);
        close(_fd);
        return EXIT_FAILURE;
    }
    return invalue;
}