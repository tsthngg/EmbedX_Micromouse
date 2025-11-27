#include "PID.h"

PID::PID (float p, float i, float d, float deltaTime, long long maxI, long long minI, long long maxOut, long long minOut)
    : Kp(p), Ki(i), Kd(d), dt(deltaTime), maxIntegral(maxI), minIntegral(minI), minOutput(minOut), maxOutput(maxOut) {}
 
// Tính toán giá trị PID
float PID::tinhtoan(long long setpoint, long long measured){        
    
    long long error = setpoint - measured;    //measured là giá trị đo được trong thực tế, setpoint là giá trị mong muốn 

    float newIntegral = integral + error * dt;       // Integral là tích phân, để cộng dồn sai số theo thời gian để giảm thiểu sai số về lâu dài 

    newIntegral = limit(newIntegral, minIntegral, maxIntegral);    // Tránh cộng dồn tích phân quá lớn so với sai số mà motor bù được

    float derivative = (error - prevError) / dt;            //derivative là đạo hàm, tính sự thay đổi sai số theo thời gian

    float output = Kp * error + Ki * newIntegral + Kd * derivative;    

    output = limit(output, minOutput, maxOutput);   

// Chỉ cập nhật output trong tầm motor bù được

    if (output != maxOutput && output != -maxOutput){
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

//Hàm giới hạn đầu ra
long long limit (long long currentPwm, const long long minPwm, const long long maxPwm){
    currentPwm = (currentPwm > maxPwm) ? maxPwm : (currentPwm < minPwm) ? minPwm : currentPwm;
    return currentPwm; 
}
