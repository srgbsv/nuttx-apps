#pragma once

#include <stdatomic.h>
#include "CanState.hpp"

namespace Snow {

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

}