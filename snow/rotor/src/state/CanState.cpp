#include "state/CanState.hpp"

void CanState::canInit(bool init)
{
    _inited = init;
}

void CanState::canRun(bool isRunning) 
{
    _running = isRunning;
}

bool CanState::isRunning() {
    return _running;
}

bool CanState::isInited() {
    return _inited;
}

CanState::CanState () {
}

CanState::~CanState() {
    
}