#ifndef PID_H
#define PID_H

#include <algorithm>

class PID {
public:
    float Kp, Ki, Kd;
    float integral = 0;
    float prevError = 0;
    float dt;
    float maxIntegral;
    float minIntegral;
    float maxOutput;
    long long clamp(long long newIntegral, const long long minIntergral, const long long maxIntergral);
    PID(float p, float i, float d, float  deltaTime, float maxI, float minI, float maxOut);
    float tinhtoan(float setpoint, float measured);
    void reset();
    void setDt(float newDt);
};

#endif

