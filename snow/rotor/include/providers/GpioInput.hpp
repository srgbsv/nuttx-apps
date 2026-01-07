#pragma once

#include <string.h>

class GpioInput {
    int             _fd = -1;
    bool            _inited = false;
    // fixed-size buffer for embedded builds (no std::string)
    char            _devpath[16];

    public:
    GpioInput(const char * devpath);
    GpioInput() { _devpath[0] = '\0'; };
    bool init(const char * devpath);
    ~GpioInput();
    [[nodiscard]] bool isInit() const { return _inited; }

    int getState();
    private:
};