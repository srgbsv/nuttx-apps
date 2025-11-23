#pragma once

#include <string>

class GpioOutput {
    private:
    int             _fd;
    bool            _inited = false;
    char *          _devpath;
    bool            _default = true;

    public:
    GpioOutput() {};
    bool init(const char * devpath);
    ~GpioOutput();
    [[nodiscard]] bool isInit() const { return _inited; }

    bool setValue(bool value);
};
