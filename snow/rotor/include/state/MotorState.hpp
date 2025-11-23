#pragma once

class MotorState
{
private:
    int _value = 0;

public:
    int getValue();
    void setValue(int value);
};