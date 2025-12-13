#include "Motor_control.h"
#include <Arduino.h>
#include "PID.h"


  void setup(){
    setupMotor_control();
    Serial.begin(9600);
      }
    PID pidLeft(1.2,0.05,0.8,0,255,-255,255,-255);
    PID pidRight(1.2,0.05,0.8,0,255,-255,255,-255);
    long long setpoint;
    long long measuredLeft;
    long long measuredRight;
    long long prevTime = 0;
    long long errorL;
    long long errorR;
    float newIntegralL;
    float newIntegralR;
    float derivativeL;
    float derivativelR;
    float outputLeft;
    float outputRight;

  void loop(){ 
    setpoint = 9600;
    long long currentTime = millis();
    pidLeft.dt = (currentTime - prevTime)/1000;
    pidRight.dt = (currentTime - prevTime)/1000;
    prevTime = currentTime;

    measuredLeft = encoder1Value;

    errorL = setpoint - measuredLeft; //measured là giá trị đo được trong thực tế, setpoint là giá trị mong muốn 

    newIntegralL = pidLeft.integral + errorL *pidLeft.dt;       // Integral là tích phân, để cộng dồn sai số theo thời gian để giảm thiểu sai số về lâu dài 

    newIntegralL = limit(newIntegralL, -255, 255);    // Tránh cộng dồn tích phân quá lớn so với sai số mà motor bù được

    derivativeL= (errorL - pidLeft.prevError) / pidLeft.dt;            //derivative là đạo hàm, tính sự thay đổi sai số theo thời gian

    outputLeft = pidLeft.Kp * errorL + pidLeft.Ki * newIntegralL + pidLeft.Kd * derivativeL;    

    outputLeft = limit(outputLeft, -255 , 255);   

// Chỉ cập nhật output trong tầm motor bù được

    if (outputLeft != pidLeft.maxOutput && outputLeft != -pidLeft.maxOutput){
        pidLeft.integral = newIntegralL;
    }
    pidLeft.prevError = errorL; //prevError là sai số của lần tính trước   

    measuredRight = encoder2Value;

    errorR = setpoint - measuredRight; //measured là giá trị đo được trong thực tế, setpoint là giá trị mong muốn 

    newIntegralR = pidRight.integral + errorR *pidRight.dt;       // Integral là tích phân, để cộng dồn sai số theo thời gian để giảm thiểu sai số về lâu dài 

    newIntegralR = limit(newIntegralR, -255, 255);    // Tránh cộng dồn tích phân quá lớn so với sai số mà motor bù được

    derivativelR = (errorR - pidRight.prevError) / pidRight.dt;            //derivative là đạo hàm, tính sự thay đổi sai số theo thời gian

    outputRight = pidRight.Kp * errorR + pidRight.Ki * newIntegralR + pidRight.Kd * derivativelR;    

    outputRight = limit(outputRight, -255 , 255);   

// Chỉ cập nhật output trong tầm motor bù được

    if (outputRight != pidRight.maxOutput && outputRight != -pidRight.maxOutput){
        pidRight.integral = newIntegralR;
    }
    pidRight.prevError = errorR; //prevError là sai số của lần tính trước   
            if(outputLeft >= 0){
            digitalWrite(motorDir1_L, LOW);
            digitalWrite(motorDir2_L, HIGH);
            ledcWrite(0,outputLeft);
        }
        else {
            digitalWrite(motorDir1_L,HIGH);
            digitalWrite(motorDir2_L,LOW);
            ledcWrite(0,-outputLeft);
        }
        if(outputRight >= 0){
            digitalWrite(motorDir1_R,HIGH);
            digitalWrite(motorDir2_R, LOW);
            ledcWrite(1, outputRight);
        }
        else{
            digitalWrite(motorDir1_R, LOW);
            digitalWrite(motorDir2_R, HIGH);
            ledcWrite(1, -outputRight);
          }

      if(((abs(encoder1Value - setpoint) < 500)) && ((abs(encoder2Value - setpoint) < 500 ))){ 
        pidLeft.reset();
        pidRight.reset();
        prevTime = 0;
        encoder1Value = 0;
        encoder2Value = 0;
        ledcWrite(0,0);
        ledcWrite(1,0);
        delay(3000);
      }
      
  
  Serial.print("Encoder 1 value : "); Serial.println(encoder1Value); 
  Serial.print("Encoder 2 Value : "); Serial.println(encoder2Value);
}
 
