//
// Created by sergey on 01.09.24.
//

#include <nuttx/config.h>

#include <sys/types.h>
#include <sys/ioctl.h>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include <syslog.h>
#include <nuttx/timers/capture.h>

#include "providers/PwmInput.hpp"
#include "Config.hpp"
#include "Logging.hpp"

PwmInput::PwmInput (const char * devpath) {
    init (devpath);
}

PwmInput::~PwmInput() {
    close(_fd);
}

bool PwmInput::init(const char * devpath) {
     if (devpath) {
        strncpy(_devpath, devpath, sizeof(_devpath) - 1);
        _devpath[sizeof(_devpath) - 1] = '\0';
    } else {
        _devpath[0] = '\0';
    }
    _fd = open(devpath, O_RDONLY);
    //Check for error opening the deviceuu
    if (_fd < 0) {
        snowerror("Error when opening device %s\n", devpath);
        return false;
    }
    snowinfo("PwmInput: device %s opened successfully, PID: %d\n", devpath, getpid());

    _inited = true;
    return true;
}

bool PwmInput::init (const char* devpath, int min, int max) {
    _min = min;
    _max = max;
    return init (devpath);
}

int PwmInput::getDutyCycle () {
    if (!_inited) {
        snowerror("PwmInput: device %s is not inited\n", _devpath);

        return 0;
    }
    //Check for error opening the device
    int8_t dutycycle = 0;
    if (_fd <= 0) {
        syslog (LOG_ERR, "Wrong file descriptor");
        return 0;
    }

	int ret = ioctl(_fd, CAPIOC_DUTYCYCLE,
    	(unsigned long)((uintptr_t)&dutycycle));

	if (ret < 0) {
        snowerror("PwmInput: Device %s, PID: %d,  ioctl(CAPIOC_DUTYCYCLE) failed: %d\n", _devpath, getpid(), errno);

        return 0;
    }
    syslog(LOG_DEBUG, "Duty: %d", dutycycle);
	return dutycycle;
}
int PwmInput::getDutyCycleNormalized () {
    auto duty = getDutyCycle();
    return float (duty - _min) / (_max - _min) * 100;
}
