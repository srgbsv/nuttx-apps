//
// Created by sergey on 01.09.24.
//

#ifndef SNOWBLOWER_PWMINPUT_H
#define SNOWBLOWER_PWMINPUT_H


#include "cstdio"
#include "string"

class PwmInput {

    private:
    int _fd;
    bool _inited = false;
    std::string _devpath;

    public:
    PwmInput (const char* devpath);
    PwmInput (){};
    ~PwmInput ();

    bool init (const char* devpath);

    [[nodiscard]] bool isInit () const {
        return _inited;
    }

    int getDutyCycle ();
};


#endif // SNOWBLOWER_PWMINPUT_H
