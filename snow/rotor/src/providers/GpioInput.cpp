//
// Created by sergey on 10.10.24.
//
#include <errno.h>
#include <fcntl.h>
#include <nuttx/ioexpander/gpio.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <syslog.h>
#include <unistd.h>

#include "providers/GpioInput.hpp"
#include "Logging.hpp"

GpioInput::GpioInput (const char* devpath) {
    _devpath[0] = '\0';
    init (devpath);
}

GpioInput::~GpioInput () {
    if (_fd >= 0) {
        close(_fd);
        _fd = -1;
    }
}

bool GpioInput::init (const char* devpath) {
    // safe copy into fixed buffer
    if (devpath) {
        strncpy(_devpath, devpath, sizeof(_devpath) - 1);
        _devpath[sizeof(_devpath) - 1] = '\0';
    } else {
        _devpath[0] = '\0';
    }

    _fd = open(_devpath, O_RDONLY);
    if (_fd < 0) {
        snowerror("open %s failed: %s\n", _devpath, strerror(errno));
        _inited = false;
        _fd = -1;
        return false;
    }
    snowinfo("GpioInput: device %s opened successfully\n", _devpath);
    _inited = true;
    return true;
}

int GpioInput::getState () {
    bool invalue = false;

    int ret = ioctl(_fd, GPIOC_READ, (unsigned long)((uintptr_t)&invalue));
    if (ret < 0)
    {
        int errcode = errno;
        snowerror("ERROR: Failed to read value from %s: %d\n",
        _devpath, errcode);
        return EXIT_FAILURE;
    }
    return invalue;
}