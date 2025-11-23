//
// Created by sergey on 10.10.24.
//
#include <errno.h>
#include <fcntl.h>
#include <nuttx/ioexpander/gpio.h>
#include <stdio.h>
#include <string>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <syslog.h>
#include <unistd.h>

#include "providers/GpioInput.hpp"
#include "Logging.hpp"

GpioInput::GpioInput (const char* devpath) {
    init (devpath);
}

GpioInput::~GpioInput () {
    close(_fd);
}

bool GpioInput::init (const char* devpath) {
    strcpy(_devpath, devpath);
    _fd = open(devpath, O_RDONLY);
    if (_fd < 0) {
        snowerror("open %s failed: %s", _devpath, strerror(errno));
        return false;
    }
    snowinfo("GpioInput: device %s opened successfully", _devpath);
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
        close(_fd);
        return EXIT_FAILURE;
    }
    return invalue;
}