#include "cambienimu.h"

// Khởi tạo đối tượng MPU6050
MPU mpu;

// Góc nghiêng ban đầu
float angleZ = 0.0f;

// Hệ số chuyển đổi từ raw sang đơn vị vật lý
const float ACC_SCALE = 1.0f / 16384.0f;  // ±2g mode
const float GYRO_SCALE = 1.0f / 131.0f;   // ±250°/s mode

// Biến thời gian để tính dt
unsigned long lastTime = 0;

// Trạng thái chuyển động hiện tại
int currentMotionState = 0;

// Bộ nhớ cho lọc trung bình trượt (Moving Average)
const int accWindowSize = 5;
const int gyroWindowSize = 10;
float accXBuffer[accWindowSize] = {0};
float accYBuffer[accWindowSize] = {0};
float accZBuffer[accWindowSize] = {0};
float gyroZBuffer[gyroWindowSize] = {0};
int accIndex = 0;
int gyroIndex = 0;

// Hàm lọc trung bình trượt cho acc
float movingAverage(float* buffer, float newValue) {
  buffer[accIndex] = newValue;
  float sum = 0.0f;
  for (int i = 0; i < accWindowSize; i++) {
    sum += buffer[i];
  }
  return sum / accWindowSize;
}
// Hàm lọc trung bình trượt cho gyro
float movingAverageGyro(float* buffer, float newValue) {
  buffer[gyroIndex] = newValue;
  float sum = 0.0f;
  for (int i = 0; i < gyroWindowSize; i++) {
    sum += buffer[i];
  }
  return sum / gyroWindowSize;
}

// Hàm khởi tạo cảm biến MPU6050
void setupIMU() {
  Wire.begin(21, 22);         // Khởi tạo I2C với chân SDA/SCL tùy ESP32
  mpu.begin();           // Khởi tạo MPU6050
  mpu.calcGyroOffsets();        // Hiệu chỉnh lệch gyro

  lastTime = micros();        // Ghi nhận thời gian bắt đầu
}
void updateIMU() {
  int16_t ax, ay, az;
  int16_t gx, gy, gz;

  mpu.update();  // Cập nhật dữ liệu cảm biến
  
  // Không sử dụng gia tốc kế trong 3 giây đầu để tránh nhiễu do khởi động - Hiệu chỉnh
  if (millis()>3000){
    ax = mpu.getAccX()
    ay = mpu.getAccY();
    az = mpu.getAccZ();
    gz = mpu.getGyroZ();
  } else {
    gz = mpu.getGyroZ() 
    ax = 0;
    ay = 0;   
    az = 0;
  }
  
  // Chuyển đổi dữ liệu
  float rawAccX = ax * ACC_SCALE;
  float rawAccY = ay * ACC_SCALE;
  float rawAccZ = az * ACC_SCALE;
  float rawgyroZ = gz * GYRO_SCALE;

 // Lọc trung bình trượt cho acc để giảm nhiễu
  float AccX = movingAverage(accXBuffer, rawAccX);
  float AccY = movingAverage(accYBuffer, rawAccY);
  float AccZ = movingAverage(accZBuffer, rawAccZ);
  accIndex = (accIndex + 1) % accWindowSize;  // Di chuyển vị trí ghi tiếp theo
  // Lọc trung bình trượt cho gyro để giảm nhiễu
  float GyroZ = movingAverageGyro(gyroZBuffer, rawgyroZ);
  gyroIndex = (gyroIndex + 1) % gyroWindowSize;  // Di chuyển vị trí ghi tiếp theo

  // Tính thời gian
  unsigned long currentTime = micros();
  float dt = (currentTime - lastTime) / 1000000.0f;
  lastTime = currentTime;

  // Tích phân tốc độ quay để lấy góc quay
  angleZ += GyroZ * dt;

  // Tính tổng gia tốc để phát hiện va chạm
  float accMagnitude = sqrt(AccX * AccX + AccY * AccY + AccZ * AccZ);
  bool isCollision = accMagnitude > 2.5f;  // Ngưỡng va chạm

  // Xác định trạng thái chuyển động
  if (isCollision) {
    currentMotionState = MOTION_COLLISION;
  } else if (fabs(angleZ) < 20.0f) {
    if (AccY > 0.2f) {
      currentMotionState = MOTION_STRAIGHT_ACCEL;
    } else if (AccY < -0.2f) {
      currentMotionState = MOTION_STRAIGHT_DECEL;
    } else {
      currentMotionState = MOTION_STRAIGHT_CONSTANT;
    }
  } else {
    if (fabs(angleZ) > 145.0f && fabs(GyroZ) < 10.0f ) {
      currentMotionState = MOTION_TURN_AROUND;
      angleZ = 0.0f;
    } else if (angleZ > 0.0f) {
      currentMotionState = MOTION_TURN_RIGHT;
    } else {
      currentMotionState = MOTION_TURN_LEFT;
    }
  }

 // delay(5) - Tần số cập nhật ~200Hz - Để ngoài vòng loop
}
