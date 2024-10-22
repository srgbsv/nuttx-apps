//
// Created by sergey on 10.10.24.
//

#ifndef SNOWBLOWER_GPIOINPUT_H
#define SNOWBLOWER_GPIOINPUT_H


#include "string"
class GpioInput {
    int             _fd;
    bool            _inited = false;
    std::string          _devpath;

    public:
    GpioInput(const char * devpath);
    GpioInput() {};
    bool init(const char * devpath);
    ~GpioInput();
    [[nodiscard]] bool isInit() const { return _inited; }

    int getState();
    private:
};


#endif // SNOWBLOWER_GPIOINPUT_H
