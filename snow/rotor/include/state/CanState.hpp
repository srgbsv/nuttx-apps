#ifndef SNOW_ROTOR_CAN_STATE
#define SNOW_ROTOR_CAN_STATE

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


#endif //SNOW_ROTOR_CAN_STATE