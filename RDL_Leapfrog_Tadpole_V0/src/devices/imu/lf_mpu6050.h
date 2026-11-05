#pragma once

#include "../../core/types.h"

class MPU6050
{
public:
    explicit MPU6050(uint8_t addr);

    // Init sensor and return status bool
    bool begin();

    // Read sensor data and put it in the ImuData struct that was input to the function
    // e.g. imu1.read(primaryImu);
    bool read(ImuData& data);

    bool readAccel(ImuData& data);
    bool readGyro(ImuData& data);

private:
    uint8_t addr_;
};