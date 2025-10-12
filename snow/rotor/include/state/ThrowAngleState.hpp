#pragma once

class ThrowAngleState
{
private:
    int _angle = 0;

public:
    int getAngle() { return _angle; }
    void setAngle(int angle) { _angle = angle; }
};