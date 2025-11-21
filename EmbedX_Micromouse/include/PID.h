#define PID_H
#define PID_H

#include <algorithm>

class PID {
public:
    float Kp, Ki, Kd;
    float integral = 0;
    float prevError = 0;
    float dt;
    float maxIntegral;
    float maxOutput;

    PID(float p, float i, float d, float  deltaTime, float maxI = 1000.0, float maxOut = 255.0);
    float tinhtoan(float setpoint, float measured);
    void reset();
    void setDt(float newDt);
};

#endif

