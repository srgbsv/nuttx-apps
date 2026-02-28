#pragma once

class CanState
{
private:
    bool _inited  = false;
    bool _running = false;
    
public:
    void canInit(bool init);
    void canRun(bool run);

    bool isInited();
    bool isRunning();

    CanState(/* args */);
    ~CanState();
};