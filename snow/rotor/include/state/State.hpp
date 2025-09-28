#pragma once

#include <stdatomic.h>
#include "CanState.hpp"

namespace snow 
{

namespace rotor 
{

class State
{
private:
    /* data */
    CanState _can_state;
    
public:
    CanState getCanState();
    State(/* args */);
    ~State();
};


State::State(/* args */)
{

}

State::~State()
{
}

} // namespace rotor

} // namespace snow