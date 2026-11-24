#pragma once

#include <Arduino.h>

struct SimState
{
    uint32_t vehicleId = 0;
    float positionM[3] = { 0.0f, 0.0f, 0.0f };
    float velocityMps[3] = { 0.0f, 0.0f, 0.0f };
    float orientationEulerDeg[3] = { 0.0f, 0.0f, 0.0f };
    float accelerationBodyMps2[3] = { 0.0f, 0.0f, 0.0f };
    float angularVelocityBodyRadps[3] = { 0.0f, 0.0f, 0.0f };
    float altitudeM = 0.0f;
    float pressurePa = 101325.0f;
    float temperatureC = 15.0f;
};

struct SimTarget
{
    float positionM[3] = { 0.0f, 0.0f, 0.0f };
};

struct SimActuatorCommand
{
    float thrustNewtons = 0.0f;
    float gimbalXDegrees = 0.0f;
    float gimbalYDegrees = 0.0f;
};
