#include <Wire.h>

bool beginMPU6050(){

#define MPU6050_ADDRESS 0x68

    Wire.begin();
    Wire.beginTransmission(MPU6050_ADDRESS);
    Wire.write(0x6B);
    Wire.write(0x00);

    // 0 is sucessful return code
    if(Wire.endTransmission(true) != 0){return false;}

    // Successful sensor wake
    return true;

} // end beginMPU6050