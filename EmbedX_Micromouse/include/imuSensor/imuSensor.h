#ifndef IMU_H
#define IMU_H

#include <Wire.h>
#include <MPU6050_light.h> // Thư viện MPU6050 bạn đang dùng

// Đối tượng cảm biến
MPU mpu;

// Góc nghiêng các trục
extern float angleX;
extern float angleY;
extern float angleZ;

// Hằng số lọc
extern const float alpha;

// Hệ số chuyển đổi
extern const float ACC_SCALE;
extern const float GYRO_SCALE;

// Biến thời gian
extern unsigned long lastTime;

// Cờ kiểm tra acc đã sẵn sàng
extern bool accReady;

// Bộ nhớ cho lọc trung bình trượt
extern const int accWindowSize;
extern float accXBuffer[];
extern float accYBuffer[];
extern float accZBuffer[];
extern int accIndex;

// Trạng thái chuyển động
extern int currentMotionState;

// Khai báo các hàm
extern float movingAverage(float* buffer, float newValue);
extern void setupIMU();
extern void updateIMU();

#endif


