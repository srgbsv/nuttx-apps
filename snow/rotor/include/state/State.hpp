#pragma once

#include <stdatomic.h>
#include <pthread.h>
#include "CanState.hpp"
#include "RotateState.hpp"
#include "MotorState.hpp"
#include "EjectionState.hpp"

enum CommonState {
    STATE_DISABLED = 0,
    STATE_TESTING  = 1,
    STATE_RUNNING  = 2,
    STATE_ERROR    = 3, 
};

class State
{
private:
    /* data */
    CanState               _can_state;
    RotateState            _rotate_state;
    MotorState             _motor_state;
    EjectionState          _ejection_state;
    bool                   _is_enabled = true;
    pthread_mutex_t        _state_mutex;
    
public:
    CanState&           getCanState();
    RotateState&        getRotateState();
    MotorState&         getMotorState();
    EjectionState&      getEjectionState();
    bool                getIsEnabled();

    int setActuatorValue(int index, int value);
    
    void lock() { 
        //pthread_mutex_lock(&_state_mutex); 
    }
    void unlock() { 
        //pthread_mutex_unlock(&_state_mutex); 
    }

    State(/* args */);
    ~State();
};
