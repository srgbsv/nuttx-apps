#pragma once

#include <string>

class EncoderProvider {
    int             _fd;
    bool            _inited = false;
    char            _devpath[16];

public:
    EncoderProvider(const char * devpath);
    EncoderProvider() {};
    bool init(const char * devpath);
    ~EncoderProvider();
    [[nodiscard]] bool isInit() const { return _inited; }

    void resetCounter();

    int getState();

};