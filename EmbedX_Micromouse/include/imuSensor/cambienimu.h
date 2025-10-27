#ifndef IMU_H
#define IMU_H

#include <Arduino.h>
#include <MPU6050_light.h>
#include <Wire.h>

// Đối tượng cảm biến
extern MPU mpu;

// Góc nghiêng các trục
extern float angleZ;

// Hệ số chuyển đổi
extern const float ACC_SCALE;
extern const float GYRO_SCALE;

// Biến thời gian
extern unsigned long lastTime;

// Trạng thái chuyển động
extern volatile int currentMotionState;

// Quy định trạng thái chuyển động bằng số nguyên
  enum MotionState {
  MOTION_STOPPED = 0,              // Xe đứng yên
  MOTION_COLLISION = 1,           // Va chạm với tường
  MOTION_STRAIGHT_ACCEL = 2,       // Đi thẳng và tăng tốc
  MOTION_STRAIGHT_DECEL = 3,       // Đi thẳng và giảm tốc
  MOTION_STRAIGHT_CONSTANT = 4,    // Đi thẳng đều
  MOTION_TURN_LEFT = 5,            // Rẽ trái
  MOTION_TURN_RIGHT = 6,           // Rẽ phải
  MOTION_TURN_AROUND = 7,           // Quay 180 độ (quay ngược)

  };

// Khai báo các hàm
float movingAverage(float* buffer, float newValue);
void setupIMU();
void updateIMU();

#endif