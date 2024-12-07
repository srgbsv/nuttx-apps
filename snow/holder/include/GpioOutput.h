//
// Created by sergey on 10.10.24.
//

#ifndef HOLDER_GPIOOUTPUT_H
#define HOLDER_GPIOOUTPUT_H


#include "string"
class GpioOutput {
    private:
    int             _fd;
    bool            _inited = false;
    std::string     _devpath;
    bool            _default = false;

    public:
    GpioOutput() {};
    bool init(const char * devpath);
    ~GpioOutput();
    [[nodiscard]] bool isInit() const { return _inited; }

    bool setValue(bool value);
};


#endif // HOLDER_GPIOOUTPUT_H
