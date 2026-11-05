#include <Wire.h>
#include "lf_mpu6050.h"


enum MPU6050_Registers : uint8_t
{ 
    PWR_MGMT_1  = 0x6B, // power management 1 (global)
    GYRO_XOUT_H = 0x43, // x-axis gyroscope - high byte
    ACCEL_XOUT_H = 0x3B, // x-axis accelerometer - high byte
    TEMP_OUT_H = 0x41 // temperature - high byte
};

MPU6050::MPU6050(uint8_t addr)
{
    addr_ = addr;
}

bool MPU6050::begin()
{

    // Begin sensor on I2C
    Wire.begin();
    Wire.beginTransmission(addr_);
    Wire.write(0x6B);
    Wire.write(0x00);

    // 0 is sucessful return code
    if(Wire.endTransmission(true) != 0){return false;}

    // Successful sensor wake
    return true;
}

// Read data from both accelerometer and gyroscope in one go
bool MPU6050::read(ImuData& data)
{

    bool accelStatus = readAccel(data);
    bool gyroStatus = readGyro(data);

    return accelStatus && gyroStatus; // Make sure both processes succeeded
}

// Read raw accelerometer data
bool MPU6050::readAccel(ImuData& data)
{

    Wire.beginTransmission(addr_);
    Wire.write(ACCEL_XOUT_H); // used as starting address to read all other values
    if(Wire.endTransmission(false) != 0){return false;} // Check if read was successful and keep sensor active

    Wire.requestFrom(addr_, 6, true); // requests 6 bytes - 3 high values and 3 low values from given address
    
    if (Wire.available() < 6) // check if at least all 6 bytes are sent
    {     
        return false; // not enough data sent from MPU6050, Sensor failure
    };

    // reads high (most significant) byte,
    // shifts 8 places to make room for low byte
    // and then bitwise OR combines the low (least significant) byte to form a 2 byte accelerometer value
    float rawAccelX = Wire.read() << 8 | Wire.read(); 
    float rawAccelY = Wire.read() << 8 | Wire.read();
    float rawAccelZ = Wire.read() << 8 | Wire.read();

    data.ax = rawAccelX;
    data.ay = rawAccelY;
    data.az = rawAccelZ;

    return true;
}

// Read raw gyroscope data
bool MPU6050::readGyro(ImuData& data)
{

    Wire.beginTransmission(addr_); 
    Wire.write(GYRO_XOUT_H); // used as starting address to read all other values
    if(Wire.endTransmission(false) != 0){return false;} // Check if read was successful and keep sensor active

    Wire.requestFrom(addr_, 6, true); // requests 6 bytes - 3 high values and 3 low values from given address
    
    if (Wire.available() < 6) // check if at least all 6 bytes are sent
    {
        return false; // not enough data sent from MPU6050, sensor failure
    };

    // reads high (most significant) byte,
    // shifts 8 places to make room for low byte
    // and then bitwise OR combines the low (least significant) byte to form a 2 byte gyroscope value
    float rawGyroX = Wire.read() << 8 | Wire.read(); 
    float rawGyroY = Wire.read() << 8 | Wire.read();
    float rawGyroZ = Wire.read() << 8 | Wire.read();

    data.gx = rawGyroX;
    data.gy = rawGyroY;
    data.gz = rawGyroZ;
    
    return true;

}
