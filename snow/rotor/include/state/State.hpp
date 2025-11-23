#pragma once

#include <stdatomic.h>
#include "CanState.hpp"
#include "RotateState.hpp"
#include "MotorState.hpp"
#include "EjectionState.hpp"

class State
{
private:
    /* data */
    CanState               _can_state;
    RotateState            _rotate_state;
    MotorState             _motor_state;
    EjectionState          _ejection_state;
    
public:
    CanState&           getCanState();
    RotateState&        getRotateState();
    MotorState&         getMotorState();
    EjectionState&      getEjectionState();

    int setActuatorValue(int index, int value);

    State(/* args */);
    ~State();
};
