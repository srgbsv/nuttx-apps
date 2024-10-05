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

PwmInput(char * devpath) {
    fd = open(devpath, O_RDONLY);
    //Check for error opening the device
    if (fd < 0) {
        perror("Error opening device");
        exit(1);
    }
}

PwmInput::PwmInput() {

}

PwmInput::~PwmInput() {
    close(fd);
}

PwmInput::get_duty_cycle() {
	ret = ioctl(fd, CAPIOC_DUTYCYCLE,
    	(unsigned long)((uintptr_t)&dutycycle));

	if (ret < 0) {
    	printf("PwmInput: ioctl(CAPIOC_DUTYCYCLE) failed: %d\n", errno);
    }

	return ret;
}