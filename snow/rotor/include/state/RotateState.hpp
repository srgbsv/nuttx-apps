#pragma once

class RotateState
{
private:
    int _angle = 0;
    int _target_angle = 0;

public:
    int   getAngle();
    void  setAngle(int angle);
    void  setTargetAngle(int angle);
    int   getTargetAngle();
};