#pragma once

class PwmInput {

    private:
    int _fd;
    bool _inited    = false;
    char *     _devpath;
    int _min = 0;
    int _max = 100;

    
    public:
    PwmInput (const char* devpath);
    PwmInput (){};
    ~PwmInput ();

    bool init (const char* devpath);
    bool init (const char* devpath, int min, int max);

    [[nodiscard]] bool isInit () const {
        return _inited;
    }
    [[nodiscard]] int getMin () const {
        return _min;
    }
    [[nodiscard]] int getMax () const {
        return _max;
    }
    [[nodiscard]] int getMedian () const {
        return (_max - _min) / 2;
    }
    int getDutyCycle ();
    int getDutyCycleNormalized();
};