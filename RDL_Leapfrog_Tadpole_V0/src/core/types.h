#pragma once

#include <Arduino.h>

enum alertLevel {
    L_DEBUG, // grey
    L_INFO, // white
    L_PASS, // green
    L_WARN, // yellow
    L_FAIL, // red
    L_ERROR, // red, bold
    L_FATAL, // idk
    L_STATE
};

struct ImuData
{
    float ax;
    float ay;
    float az;

    float gx;
    float gy;
    float gz;
};

struct Sensors
{
    bool allOK = false;
    bool statusAccel = false;
    bool statusGyro = false;
};

struct Settings{
    bool testMode = true;
    bool showTimestamps = false;
    alertLevel minLevel = L_DEBUG; // all messages show
    bool useSerialColors = true;
};

struct EventList {
    bool preFlight = true;
    bool ready = false;
    bool abort = false;
};

struct Timestamps {
    unsigned long bootTime = 0;
    unsigned long setupCompleteTime = 0;
    unsigned long setupDuration = 0;
};

struct Pins{
    uint8_t testRead = 5;
};
