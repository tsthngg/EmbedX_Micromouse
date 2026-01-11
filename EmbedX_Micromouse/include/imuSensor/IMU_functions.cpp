#include "imuSensor/IMUSENSOR.h"

MPU6050 mpu(Wire);
float ax=0, gz=0;

const float ACC_SCALE  = 1.0f / 16384.0f;
const float GYRO_SCALE = 1.0f / 131.0f;

#define N_BUF 5
static float accBuf[N_BUF]={0};
static float gyroBuf[N_BUF]={0};

// yaw tích lũy
static float yaw=0.0f;
static unsigned long lastMicrosYaw=0;

const float COLLISION_THRESHOLD=0.3f;
const float STRAIGHT_THRESHOLD=1.0f;

void IMU_start() {                  // Riêng hàm khởi động IMU này thì để ở void setup
    Wire.begin(21,22);
    mpu.begin();
    mpu.calcGyroOffsets();
    lastMicrosYaw=micros();
    yaw=0.0f;
}

void IMU_read() {
    mpu.update();

    float rawAx = mpu.getAccX()*ACC_SCALE;
    float rawGz = mpu.getGyroZ()*GYRO_SCALE;

    for(int i=N_BUF-1;i>0;i--){
        accBuf[i]=accBuf[i-1];
        gyroBuf[i]=gyroBuf[i-1];
    }
    accBuf[0]=rawAx;
    gyroBuf[0]=rawGz;

    float sumAx=0, sumGz=0;
    for(int i=0;i<N_BUF;i++){
        sumAx+=accBuf[i];
        sumGz+=gyroBuf[i];
    }
    ax=sumAx/N_BUF;
    gz=sumGz/N_BUF;
}

bool IMU_isCollision(){ return fabs(ax) > COLLISION_THRESHOLD; }
bool IMU_isStraight(){ return fabs(gz) < STRAIGHT_THRESHOLD; }

void IMU_resetYaw(){
    yaw=0.0f;
    lastMicrosYaw=micros();
}

// Hàm tính góc quay - Gọi ra khi cần quay đầu hoặc quay góc vuông
bool IMU_turnSquare(float targetAngle){
    unsigned long now = micros();
    float dt = (now - lastMicrosYaw)/1000000.0f;
    lastMicrosYaw = now;

    yaw += gz*dt; // tích gyro Z

    if(fabs(yaw) >= fabs(targetAngle)){
        return true;  // đạt góc mong muốn
    }
    return false;     // chưa đạt
}

