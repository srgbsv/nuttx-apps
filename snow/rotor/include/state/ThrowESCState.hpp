#pragma once

class ThrowESCState
{
private:
    int _value = 0;

public:
    int getValue() { return _value; }
    void setValue(int value) { _value = value; }
};