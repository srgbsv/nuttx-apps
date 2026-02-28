#include <errno.h>
#include <fcntl.h>
#include <nuttx/ioexpander/gpio.h>
#include <nuttx/sensors/qencoder.h>
#include <cstdio>
#include <string>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <syslog.h>
#include <unistd.h>

#include "providers/EncoderProvider.hpp"
#include "Logging.hpp"


EncoderProvider::EncoderProvider (const char* devpath) {
    init (devpath);
}

bool EncoderProvider::init (const char* devpath) {
    if (devpath) {
        strncpy(_devpath, devpath, sizeof(_devpath) - 1);
        _devpath[sizeof(_devpath) - 1] = '\0';
    } else {
        _devpath[0] = '\0';
    }    _fd = open(devpath, O_RDONLY);
    if (_fd < 0) {
        snowerror("open %s failed: %s", devpath, strerror(errno));
        return false;
    }
    snowinfo("EncoderProvider: device %s opened successfully", devpath);
    _inited = true;
    return true;
}

EncoderProvider::~EncoderProvider () {
    if (_fd) {
        close(_fd);
    }
}

int EncoderProvider::getState () {
    bool invalue = false;

    int ret = ioctl(_fd, QEIOC_POSITION, (unsigned long)((uintptr_t)&invalue));
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

void EncoderProvider::resetCounter () {
    snowinfo("Resetting encoder counter for device\n");
    int ret = ioctl(_fd, QEIOC_RESET, 0);
    if (ret < 0)
    {
        int errcode = errno;
        snowerror("ERROR: Failed to reset counter on %s: %d\n",
        _devpath, errcode);
        close(_fd);
    }
}
