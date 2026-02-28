#pragma once

#include <string>

class GpioOutput {
    private:
    int             _fd;
    bool            _inited = false;
    char            _devpath[16];
    bool            _default = true;

    public:
    GpioOutput() {};
    GpioOutput(const char * devpath);
    bool init(const char * devpath);
    ~GpioOutput();
    [[nodiscard]] bool isInit() const { return _inited; }

    bool setValue(bool value);
};
