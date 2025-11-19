#include "PID.h"

PID::PID(float p, float i, float d, float deltaTime, float maxI, float maxOut)
    : Kp(p), Ki(i), Kd(d), dt(deltaTime), maxIntegral(maxI), maxOutput(maxOut) {}

// Tính toán giá trị PID
float PID::tinhtoan(float setpoint, float measured) {
    float error = setpoint - measured;                     
    float newIntegral = integral + error * dt;            
    newIntegral = std::clamp(newIntegral, -maxIntegral, maxIntegral);

    float derivative = (error - prevError) / dt;

    float output = Kp * error + Ki * newIntegral + Kd * derivative; 
    output = std::clamp(output, -maxOutput, maxOutput);   
    // Chỉ cập nhật output trong tầm motor bù được
    if (output != maxOutput && output != -maxOutput) {
        integral = newIntegral;
    }

    prevError = error;                                   
    return output;                                       
}

// Reset PID về trạng thái ban đầu
void PID::reset() {
    integral = 0;
    prevError = 0;
}

// Thay đổi dt (thời gian giữa các vòng lặp)
void PID::setDt(float newDt) {
    dt = newDt;
}
