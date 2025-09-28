#pragma once

namespace snow 
{

namespace rotor
{

class CanState
{
private:
    bool _inited = false;
    
public:
    void canInit(bool init);
    void canRun(bool run);

    CanState(/* args */);
    ~CanState();
};

CanState::CanState(/* args */)
{
    
}

CanState::~CanState()
{
}
    
} // namespace rotor

} // namespace snow