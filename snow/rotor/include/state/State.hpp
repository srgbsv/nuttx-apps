#pragma once

#include <stdatomic.h>
#include "CanState.hpp"
#include "RotateState.hpp"
#include "ThrowESCState.hpp"
#include "ThrowAngleState.hpp"

class State
{
private:
    /* data */
    CanState                _can_state;
    RotateState             _rotate_state;
    ThrowESCState           _esc_state;
    ThrowAngleState         _throw_angle_state;
    
public:
    CanState getCanState();
    State(/* args */);
    ~State();
};
