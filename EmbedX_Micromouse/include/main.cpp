#include "PID.h"

PID::PID(float p, float i, float d, float deltaTime, float maxI, float maxOut)
    : Kp(p), Ki(i), Kd(d), dt(deltaTime), maxIntegral(maxI), maxOutput(maxOut) {}

// Tính toán giá trị PID
float PID::tinhtoan(float setpoint, float measured) {        //measured là giá trị đo được trong thực tế , setpoint là giá trị mong muốn
    float error = setpoint - measured;                     
    float newIntegral = integral + error * dt;            // Integral là tích phân , để cộng dồn sai số theo thời gian để giảm thiểu sai số về lâu dài 
    newIntegral = std::clamp(newIntegral, -maxIntegral, maxIntegral);

    float derivative = (error - prevError) / dt;            //derivative là đạo hàm , tính sự thay đổi sai số theo thời gian

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

