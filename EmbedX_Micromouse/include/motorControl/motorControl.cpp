#include "Motor_control.h"
#include <Arduino.h>
#include<string.h>
// Định nghĩa chân động cơ
// Khai báo chân encoder 
const int encoder1_A = 34;
const int encoder1_B = 35;

const int encoder2_A = 32;
const int encoder2_B = 33;

// Khai báo biến encoder
volatile int encoder1Value = 0;
volatile int encoder2Value = 0;

// Chân động cơ trái
const int motorPWM_L = 18;
const int motorDir1_L = 26;   
const int motorDir2_L = 27;  

// Chân động cơ phải
const int motorPWM_R = 19;
const int motorDir1_R = 12;
const int motorDir2_R = 13;

// Khai báo thông số động cơ và tính toán xung di chuyển 
float diameter = 0.043;//mét
float distance = 0.18; // khoảng cách cần di chuyển để đi từ ô này sang ô kia là 0.18 mét
float road_turn = 0.08639379797; // khoảng cách mỗi bánh phải đi để rẽ đủ 90 độ (số liệu đo đạc thực nghiệm)
float PWM_1cycle = 1800; // số xung xuất ra khi bánh quay đủ 1 vòng (số liệu tương đối từ đo đạc thực nghiệm)

float cycles_1square = distance / (PI * diameter); // số vòng bánh xe để xe tiến được 1 ô
float cycles_turn = road_turn / (PI * diameter); // số vòng bánh xe để xe rẽ được 90 độ

float PWM_straight = cycles_1square * PWM_1cycle; // số xung cần truyền cho encoder biết thời điểm dừng khi đủ khoảng cách mong muốn
float PWM_turn = cycles_turn * PWM_1cycle*1.36; // xung cần truyền cho encoder biết thời điểm rẽ đủ 90 độ

PID::PID (float p, float i, float d)
    : Kp(p), Ki(i), Kd(d) {}
//Hàm giới hạn đầu ra
long long limit (long long currentPwm, const                       long long minPwm, const long long maxPwm){
    currentPwm = (currentPwm > maxPwm) ? maxPwm : (currentPwm < minPwm) ? minPwm : currentPwm;
    return currentPwm; 
}
// Các hàm xử lý ngắt 

void encoder1_isr(){
      int A = digitalRead(encoder1_A);
      int B = digitalRead(encoder1_B);
      if( (A == HIGH) != (B == HIGH) ){ 
            encoder1Value--;
      }
      else{
            encoder1Value++;
      }
}
void encoder2_isr(){
      int A = digitalRead(encoder2_A);
      int B = digitalRead(encoder2_B);
      if( (A == HIGH) != (B == HIGH)){
           encoder2Value ++;
      }
      else{
           encoder2Value--;
      }
}
// Cấu hình chân 
void setupMotor_control() {
  pinMode(encoder1_A, INPUT_PULLUP);
  pinMode(encoder1_B, INPUT_PULLUP);

  pinMode(encoder2_A, INPUT_PULLUP);
  pinMode(encoder2_B, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(encoder1_A), encoder1_isr, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoder2_A), encoder2_isr, CHANGE);


  pinMode(motorPWM_L, OUTPUT);
  pinMode(motorDir1_L, OUTPUT);
  pinMode(motorDir2_L, OUTPUT);

  pinMode(motorPWM_R, OUTPUT);
  pinMode(motorDir1_R, OUTPUT);
  pinMode(motorDir2_R, OUTPUT);

  ledcSetup(0, 5000, 8);
  ledcAttachPin(motorPWM_L, 0);

  ledcSetup(1, 5000, 8);
  ledcAttachPin(motorPWM_R, 1);
}
// cùng chiều kim đồng hồ encoder1 nhận giá trị dương encoder2 nhận giá trị âm
// muốn tiến bánh 1 quay thuận bánh 2 quay ngược
// muốn rẽ trái bánh 1 quay thuận bánh 2 quay thuận
// muốn rẽ phải bánh 1 quay ngược bánh 2 quay ngượ

long long moveForward (long long setpoint) {       
      setpoint = PWM_straight;
      return setpoint;
}
long long turnLeft(long long setpoint){
      setpoint = PWM_turn;
      return setpoint;
    }

long long turnRight(long long setpoint) {
      setpoint = PWM_turn;
      return setpoint;
}
 
void stop() {
  ledcWrite(0, 0);
  ledcWrite(1, 0);
}

long long turnBack(){
      long long setpoint = PWM_turn*1.9;
      return setpoint;
}
void motor_control(float outputLeft , float outputRight, int dirL, int dirR){
      if(dirL == 1){
           
            digitalWrite(motorDir1_L, LOW);
            digitalWrite(motorDir2_L, HIGH);
            ledcWrite(0,outputLeft);
      }
      
      if(dirL == 0){
      
            digitalWrite(motorDir1_L, HIGH);
            digitalWrite(motorDir2_L, LOW);
            ledcWrite(0,outputLeft);
      }           

      if(dirR == 1){
            digitalWrite(motorDir1_R,HIGH);
            digitalWrite(motorDir2_R, LOW);
            ledcWrite(1, outputRight);
   
      }
      if(dirR == 0){          
            digitalWrite(motorDir1_R,LOW);
            digitalWrite(motorDir2_R, HIGH);
            ledcWrite(1, outputRight);            
      }



  Serial.print("Encoder 1 value : "); Serial.println(encoder1Value); 
  Serial.print("Encoder 2 Value : "); Serial.println(encoder2Value);
}
    PID pidLeft(0.141 ,0,0);//0.108
    PID pidRight(0.125,0,0);//0.105850

float Motor_controlLeft (long long setpoint, long long encoder1Value, long long dt){

      static float prevErrorL = 0;
      static float integralL = 0;

      long long measuredLeft = abs(encoder1Value);

      float errorL = setpoint - measuredLeft; //measured là giá trị đo được trong thực tế, setpoint là giá trị mong muốn 

      integralL += errorL*dt;       // Integral là tích phân, để cộng dồn sai số theo thời gian để giảm thiểu sai số về lâu dài 

      integralL = limit(integralL, -255, 255);    // Tránh cộng dồn tích phân quá lớn so với sai số mà motor bù được

      float derivativeL= (errorL - prevErrorL)/dt;            //derivative là đạo hàm, tính sự thay đổi sai số theo thời gian

      float outputLeft = pidLeft.Kp * errorL + pidLeft.Ki * integralL + pidLeft.Kd * derivativeL;
      
      outputLeft = limit(outputLeft, -255 , 255);
      
      return outputLeft;
}

float Motor_controlRight (long long setpoint, long long encoder2Value, long long dt){

      static float prevErrorR = 0;
      static float integralR = 0;

      long long measuredRight = abs(encoder2Value);

      float errorR = setpoint - measuredRight; //measured là giá trị đo được trong thực tế, setpoint là giá trị mong muốn 

      integralR += errorR*dt;       // Integral là tích phân, để cộng dồn sai số theo thời gian để giảm thiểu sai số về lâu dài 

      integralR = limit(integralR, -255, 255);    // Tránh cộng dồn tích phân quá lớn so với sai số mà motor bù được

      float derivativeR= (errorR - prevErrorR) / dt;            //derivative là đạo hàm, tính sự thay đổi sai số theo thời gian

      float outputRight = pidRight.Kp * errorR + pidRight.Ki * integralR + pidRight.Kd * derivativeR;
      
      outputRight = limit(outputRight, -255 , 255);
      
      return outputRight;
}


