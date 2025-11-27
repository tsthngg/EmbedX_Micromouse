#include "PID.h"

PID::PID(float p, float i, float d, float deltaTime, float maxI, float minI, float maxOut){
    Kp = p ; Ki = i; Kd =  d ; dt = deltaTime ;  maxIntegral = maxI ;maxIntegral = -minI;  maxOutput = maxOut ;
 }

// Tính toán giá trị PID
float PID::tinhtoan(float setpoint, float measured) {        //measured là giá trị đo được trong thực tế, setpoint là giá trị mong muốn
    float error = setpoint - measured;                     
    float newIntegral = integral + error * dt;            // Integral là tích phân, để cộng dồn sai số theo thời gian để giảm thiểu sai số về lâu dài 
    newIntegral = clamp(newIntegral, minIntegral, maxIntegral);    // Tránh cộng dồn tích phân quá lớn so với sai số mà motor bù được

    float derivative = (error - prevError) / dt;            //derivative là đạo hàm, tính sự thay đổi sai số theo thời gian

    float output = Kp * error + Ki * newIntegral + Kd * derivative;    
    output = clamp(output, -maxOutput, maxOutput);     
    // Chỉ cập nhật output trong tầm motor bù được

    if (output != maxOutput && output != -maxOutput) {
        integral = newIntegral;
    }

    prevError = error; //prevError là sai số của lần tính trước               
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
long long clamp (long long newIntegral, const long long minIntegral, const long long maxIntegral){
newIntegral = (newIntegral > maxIntegral) ? maxIntegral : (newIntegral < minIntegral) ? minIntegral : newIntegral;
return newIntegral; 
}
