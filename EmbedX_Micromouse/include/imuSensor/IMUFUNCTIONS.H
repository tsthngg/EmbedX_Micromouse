#ifndef IMUSENSOR_H
#define IMUSENSOR_H

#include <Wire.h>
#include <MPU6050_light.h>

void IMU_start();
void IMU_read();
bool IMU_isCollision();
bool IMU_isStraight();

// quay góc vuông (90°)
void IMU_resetYaw();
bool IMU_turnSquare(float targetAngle); 

extern float ax;
extern float gz;

#endif
