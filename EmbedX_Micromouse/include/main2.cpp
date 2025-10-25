#include "imu.h"

// Khởi tạo đối tượng MPU6050
MPU mpu;

// Góc nghiêng ban đầu
float angleX = 0.0f;
float angleY = 0.0f;
float angleZ = 0.0f;

// Hằng số lọc cho Complementary Filter
const float alpha = 0.97f;

// Hệ số chuyển đổi từ raw sang đơn vị vật lý
const float ACC_SCALE = 1.0f / 16384.0f;  // ±2g mode
const float GYRO_SCALE = 1.0f / 131.0f;   // ±250°/s mode

// Biến thời gian để tính dt
unsigned long lastTime = 0;

// Biến kiểm tra đã sẵn sàng dùng acc chưa
bool accReady = false;

// Trạng thái chuyển động hiện tại
int currentMotionState = 0;


// Bộ nhớ cho lọc trung bình trượt (Moving Average)
const int accWindowSize = 5;
float accXBuffer[accWindowSize] = {0};
float accYBuffer[accWindowSize] = {0};
float accZBuffer[accWindowSize] = {0};
int accIndex = 0;

// Hàm lọc trung bình trượt cho acc
float movingAverage(float* buffer, float newValue) {
  buffer[accIndex] = newValue;
  float sum = 0.0f;
  for (int i = 0; i < accWindowSize; i++) {
    sum += buffer[i];
  }
  return sum / accWindowSize;
}

// Hàm khởi tạo cảm biến MPU6050
void setupIMU() {
  Wire.begin(21, 22);         // Khởi tạo I2C với chân SDA/SCL tùy ESP32
  mpu.initialize();           // Khởi tạo MPU6050
  mpu.CalibrateGyro();        // Hiệu chỉnh lệch gyro

  if (!mpu.testConnection()) {
    while (1);                // Nếu không kết nối được thì dừng chương trình
  }

  lastTime = micros();        // Ghi nhận thời gian bắt đầu
}

// Hàm cập nhật dữ liệu từ MPU6050 và tính góc nghiêng
void updateIMU() {
  int16_t ax, ay, az;         // Dữ liệu gia tốc thô
  int16_t gx, gy, gz;         // Dữ liệu gyro thô

  // Chờ 3 giây đầu để cảm biến ổn định trước khi dùng acc - Tùy chỉnh tùy vào thời gian rung của xe ban đầu
  if (!accReady && millis() > 3000) {
    accReady = true;
  }

  // Đọc dữ liệu từ cảm biến
  if (accReady) {
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);  // Lấy cả acc + gyro
  } else {
    mpu.getRotation(&gx, &gy, &gz);                // Chỉ lấy gyro
    ax = ay = az = 0;                              // Gán acc tạm để tránh rác
  }

  // Chuyển đổi dữ liệu thô sang đơn vị vật lý
  float rawAccX = ax * ACC_SCALE;
  float rawAccY = ay * ACC_SCALE;
  float rawAccZ = az * ACC_SCALE;
  float GyroX = gx * GYRO_SCALE;
  float GyroY = gy * GYRO_SCALE;
  float GyroZ = gz * GYRO_SCALE;

  // Lọc trung bình trượt cho acc để giảm nhiễu
  float AccX = movingAverage(accXBuffer, rawAccX);
  float AccY = movingAverage(accYBuffer, rawAccY);
  float AccZ = movingAverage(accZBuffer, rawAccZ);
  accIndex = (accIndex + 1) % accWindowSize;  // Di chuyển vị trí ghi tiếp theo

  // Tính thời gian giữa hai lần đọc
  unsigned long currentTime = micros();
  float dt = (currentTime - lastTime) / 1000000.0f;  // Đổi sang giây
  lastTime = currentTime;

  // Tính góc nghiêng từ cảm biến gia tốc
  float angleAccX = atan2(AccY, sqrt(AccX * AccX + AccZ * AccZ)) * 180.0f / PI;
  float angleAccY = atan2(-AccX, sqrt(AccY * AccY + AccZ * AccZ)) * 180.0f / PI;

  // Tính góc nghiêng từ gyro bằng tích phân
  float angleGyroX = angleX + GyroX * dt;
  float angleGyroY = angleY + GyroY * dt;
  float angleGyroZ = angleZ + GyroZ * dt;

  // Kết hợp acc và gyro bằng Complementary Filter
  angleX = alpha * angleGyroX + (1.0f - alpha) * angleAccX;
  angleY = alpha * angleGyroY + (1.0f - alpha) * angleAccY;

  // Với trục Z (yaw), chỉ dùng gyro vì không có từ kế
  angleZ = angleGyroZ;

  // Quy định trạng thái chuyển động bằng số nguyên

  enum MotionState {
  MOTION_STOPPED = 0,              // Xe đứng yên
  MOTION_STRAIGHT_ACCEL = 1,       // Đi thẳng và tăng tốc
  MOTION_STRAIGHT_DECEL = 2,       // Đi thẳng và giảm tốc
  MOTION_STRAIGHT_CONSTANT = 3,    // Đi thẳng đều
  MOTION_TURN_LEFT = 4,            // Rẽ trái
  MOTION_TURN_RIGHT = 5,           // Rẽ phải
  MOTION_TURN_AROUND = 6           // Quay 180 độ (quay ngược)
};
// Xác định trạng thái chuyển động dựa trên dữ liệu gyro và acc
if (fabs(GyroZ) < 4.0f) {  // Không quay
  if (AccY > 0.2f) {
    currentMotionState = MOTION_STRAIGHT_ACCEL;
  } else if (AccY < -0.2f) {
    currentMotionState = MOTION_STRAIGHT_DECEL;
  } else {
    currentMotionState = MOTION_STRAIGHT_CONSTANT;
  }
} else {
  if (GyroZ > 150.0f || GyroZ < -150.0f) {
    currentMotionState = MOTION_TURN_AROUND;  // Quay gắt → 180 độ
  } else if (GyroZ > 5.0f) {
    currentMotionState = MOTION_TURN_RIGHT;
  } else if (GyroZ < -5.0f) {
    currentMotionState = MOTION_TURN_LEFT;
  }
}
delay(5);  // Giới hạn tần số đọc dữ liệu (~200Hz)
}
