//
// Created by sergey on 01.09.24.
//

#include "../include/PwmInput.h"
#include <nuttx/config.h>

#include <sys/types.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <syslog.h>
#include <nuttx/timers/capture.h>

PwmInput::PwmInput (const char * devpath) {
    init (devpath);
}

PwmInput::~PwmInput() {
    close(_fd);
}

bool PwmInput::init(const char * devpath) {
    _devpath = std::string(devpath);
    _fd = open(devpath, O_RDONLY);
    //Check for error opening the deviceuu
    if (_fd < 0) {
        syslog(LOG_ERR,"Error when opening device %s", devpath);
        return false;
    }
    syslog(LOG_INFO, "PwmInput: device %s opened successfully, PID: %d", devpath, getpid());

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
        syslog(LOG_ERR, "PwmInput: device %s is not inited", _devpath.c_str());

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
        syslog(LOG_ERR, "PwmInput: Device %s, PID: %d,  ioctl(CAPIOC_DUTYCYCLE) failed: %d", _devpath.c_str(), getpid(), errno);

        return 0;
    }

	return dutycycle;
}
int PwmInput::getDutyCycleNormalized () {
    auto duty = getDutyCycle();
    return float (duty - _min) / (_max - _min) * 100;
}
