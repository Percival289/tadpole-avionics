#pragma once

#include "sim_types.h"

class HoverController
{
public:
    SimActuatorCommand Update(const SimState& state, const SimTarget& target, float dt);
    void Reset();

private:
    static float Clamp(float value, float minimum, float maximum);
    static float MoveToward(float current, float target, float maximumStep);
    static float UpdateAxis(
        float error,
        float velocity,
        float dt,
        float kp,
        float ki,
        float kd,
        float integralLimit,
        float& integral
    );

    float integralX_ = 0.0f;
    float integralY_ = 0.0f;
    float integralZ_ = 0.0f;
    float commandedTiltXDegrees_ = 0.0f;
    float commandedTiltZDegrees_ = 0.0f;
};
