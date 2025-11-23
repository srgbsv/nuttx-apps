#pragma once

#include <string>

class GpioInput {
    int             _fd;
    bool            _inited = false;
    char *          _devpath;

    public:
    GpioInput(const char * devpath);
    GpioInput() {};
    bool init(const char * devpath);
    ~GpioInput();
    [[nodiscard]] bool isInit() const { return _inited; }

    int getState();
    private:
};