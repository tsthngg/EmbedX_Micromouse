#ifndef PID_H
#define PID_H
class PID {
public:
    float Kp, Ki, Kd;
    float integral = 0;
    float prevError = 0;
    float dt;
    long long maxIntegral;
    long long minIntegral;
    long long maxOutput;
    long long minOutput;
    PID(float p, float i, float d, float  deltaTime, long long  maxI, long long minI, long long maxOut, long long minOut);
    float tinhtoan(long long setpoint, long long measured);
    void reset();
    void setDt(float newDt);
};

//Hàm giới hạn đầu ra 
long long limit (long long currentPwm, const long long minPwm, const long long maxPwm);

#endif
