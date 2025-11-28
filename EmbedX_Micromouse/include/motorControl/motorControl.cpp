#include "Motor_control.h"
#include "PID.h"
#include <Arduino.h>
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
float PWM_1cycle = 1060; // số xung xuất ra khi bánh quay đủ 1 vòng (số liệu tương đối từ đo đạc thực nghiệm)

float cycles_1square = distance / (PI * diameter); // số vòng bánh xe để xe tiến được 1 ô
float cycles_turn = road_turn / (PI * diameter); // số vòng bánh xe để xe rẽ được 90 độ

float PWM_straight = cycles_1square * PWM_1cycle; // số xung cần truyền cho encoder biết thời điểm dừng khi đủ khoảng cách mong muốn
float PWM_turn = cycles_turn * PWM_1cycle; // xung cần truyền cho encoder biết thời điểm rẽ đủ 90 độ

// Các hàm xử lý ngắt
void encoder1_isr(){
      int A = digitalRead(encoder1_A);
      int B = digitalRead(encoder1_B);
      if( (A == HIGH) != (B == HIGH) ){
            encoder1Value++;
      }
      else{
            encoder1Value--;
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
// muốn rẽ phải bánh 1 quay ngược bánh 2 quay ngược
   
    PID pidLeft(0,0,0,0,255,-255,255,-255);
    PID pidRight(0,0,0,0,255,-255,255,-255);
void goStraight (long long setpointLeft,long long setpointRight) {       
    
    encoder1Value = 0; // reset giá trị encoder 1 sau mỗi lần gọi hàm
    encoder2Value = 0; // reset giá trị encoder 2 sau mỗi lần gọi hàm  
    long long prevTime = 0;

    while (((abs(encoder1Value)) <= setpointLeft) && ((abs(encoder2Value)) <= setpointRight)){
        long long currentTime = millis();
        pidLeft.dt = currentTime - prevTime;
        pidRight.dt = currentTime - prevTime;
        prevTime = currentTime;

        int pwmLeft = pidLeft.tinhtoan(setpointLeft, encoder1Value);
        int pwmRight = pidRight.tinhtoan(setpointRight, encoder2Value);
        
        if(pwmLeft >= 0){
            digitalWrite(motorDir1_L, LOW);
            digitalWrite(motorDir2_L, HIGH);
            ledcWrite(0,pwmLeft);
        }
        else {
            digitalWrite(motorDir1_L,HIGH);
            digitalWrite(motorDir2_L,LOW);
            ledcWrite(0,-pwmLeft);
        }
        if(pwmRight >= 0){
            digitalWrite(motorDir1_R, HIGH);
            digitalWrite(motorDir2_R, LOW);
            ledcWrite(1, pwmRight);
        }
        else{
            digitalWrite(motorDir1_R, LOW);
            digitalWrite(motorDir2_R, HIGH);
            ledcWrite(1, -pwmRight);
        }
         static long lastPrint = 0;
        if (currentTime - lastPrint > 5) {
            Serial.print("Encoder1: "); Serial.println(encoder1Value);
            Serial.print("Encoder2: "); Serial.println(encoder2Value);
            lastPrint = currentTime;
        }
    }
    ledcWrite(1,0);
    ledcWrite(0,0);
    encoder1Value = 0;
    encoder2Value = 0;
    pidLeft.reset();
    pidRight.reset();
    
}
void turnLeft(long long setpointLeft, long long setpointRight){

 
    encoder1Value = 0; // reset giá trị encoder 1 sau mỗi lần gọi hàm
    encoder2Value = 0; // reset giá trị encoder 2 sau mỗi lần gọi hàm  
    long long prevTime = 0;

    while (((abs(encoder1Value)) <= setpointLeft) && ((abs(encoder2Value)) <= setpointRight)){
        long long currentTime = millis();
        pidLeft.dt = currentTime - prevTime;
        pidRight.dt = currentTime - prevTime;
        prevTime = currentTime;

        int pwmLeft = pidLeft.tinhtoan(setpointLeft, encoder1Value);
        int pwmRight = pidRight.tinhtoan(setpointRight, encoder2Value);
        
        if(pwmLeft >= 0){
            digitalWrite(motorDir1_L, LOW);
            digitalWrite(motorDir2_L, HIGH);
            ledcWrite(0,pwmLeft);
        }
        else {
            digitalWrite(motorDir1_L,HIGH);
            digitalWrite(motorDir2_L,LOW);
            ledcWrite(0,-pwmLeft);
        }
        if(pwmRight >= 0){
            digitalWrite(motorDir1_R, LOW);
            digitalWrite(motorDir2_R, HIGH);
            ledcWrite(1, pwmRight);
        }
        else{
            digitalWrite(motorDir1_R, HIGH);
            digitalWrite(motorDir2_R, LOW);
            ledcWrite(1, -pwmRight);
        }
         static long lastPrint = 0;
        if (currentTime - lastPrint > 5) {
            Serial.print("Encoder1: "); Serial.println(encoder1Value);
            Serial.print("Encoder2: "); Serial.println(encoder2Value);
            lastPrint = currentTime;
        }
    }
    ledcWrite(1,0);
    ledcWrite(0,0);
    encoder1Value = 0;
    encoder2Value = 0;
    pidLeft.reset();
    pidRight.reset();
    
    }

void turnRight(long long setpointLeft, long long setpointRight) {
     
    encoder1Value = 0; // reset giá trị encoder 1 sau mỗi lần gọi hàm
    encoder2Value = 0; // reset giá trị encoder 2 sau mỗi lần gọi hàm  
    long long prevTime = 0;

    while (((abs(encoder1Value)) <= setpointLeft) && ((abs(encoder2Value)) <= setpointRight)){
        long long currentTime = millis();
        pidLeft.dt = currentTime - prevTime;
        pidRight.dt = currentTime - prevTime;
        prevTime = currentTime;

        int pwmLeft = pidLeft.tinhtoan(setpointLeft, encoder1Value);
        int pwmRight = pidRight.tinhtoan(setpointRight, encoder2Value);
        
        if(pwmLeft >= 0){
            digitalWrite(motorDir1_L, HIGH);
            digitalWrite(motorDir2_L, LOW);
            ledcWrite(0,pwmLeft);
        }
        else {
            digitalWrite(motorDir1_L,LOW);
            digitalWrite(motorDir2_L,HIGH);
            ledcWrite(0,-pwmLeft);
        }
        if(pwmRight >= 0){
            digitalWrite(motorDir1_R, LOW);
            digitalWrite(motorDir2_R, HIGH);
            ledcWrite(1, pwmRight);
        }
        else{
            digitalWrite(motorDir1_R, HIGH);
            digitalWrite(motorDir2_R, LOW);
            ledcWrite(1, -pwmRight);
        }
         static long lastPrint = 0;
        if (currentTime - lastPrint > 5) {
            Serial.print("Encoder1: "); Serial.println(encoder1Value);
            Serial.print("Encoder2: "); Serial.println(encoder2Value);
            lastPrint = currentTime;
        }
    }
    ledcWrite(1,0);
    ledcWrite(0,0);
    encoder1Value = 0;
    encoder2Value = 0;
    pidLeft.reset();
    pidRight.reset();
    
    
}
 
void stop() {
  ledcWrite(0, 0);
  ledcWrite(1, 0);
}

void turnBack(long long setpointLeft, long long setpointRight){
     
    encoder1Value = 0; // reset giá trị encoder 1 sau mỗi lần gọi hàm
    encoder2Value = 0; // reset giá trị encoder 2 sau mỗi lần gọi hàm  
    long long prevTime = 0;

    while (((abs(encoder1Value)) <= setpointLeft) && ((abs(encoder2Value)) <= setpointRight)){
        long long currentTime = millis();
        pidLeft.dt = currentTime - prevTime;
        pidRight.dt = currentTime - prevTime;
        prevTime = currentTime;

        int pwmLeft = pidLeft.tinhtoan(setpointLeft, encoder1Value);
        int pwmRight = pidRight.tinhtoan(setpointRight, encoder2Value);
        
        if(pwmLeft >= 0){
            digitalWrite(motorDir1_L, HIGH);
            digitalWrite(motorDir2_L, LOW);
            ledcWrite(0,pwmLeft);
        }
        else {
            digitalWrite(motorDir1_L,LOW);
            digitalWrite(motorDir2_L,HIGH);
            ledcWrite(0,-pwmLeft);
        }
        if(pwmRight >= 0){
            digitalWrite(motorDir1_R, HIGH);
            digitalWrite(motorDir2_R, LOW);
            ledcWrite(1, pwmRight);
        }
        else{
            digitalWrite(motorDir1_R, LOW);
            digitalWrite(motorDir2_R, HIGH);
            ledcWrite(1, -pwmRight);
        }
         static long lastPrint = 0;
        if (currentTime - lastPrint > 5) {
            Serial.print("Encoder1: "); Serial.println(encoder1Value);
            Serial.print("Encoder2: "); Serial.println(encoder2Value);
            lastPrint = currentTime;
        }
    }
    ledcWrite(1,0);
    ledcWrite(0,0);
    encoder1Value = 0;
    encoder2Value = 0;
    pidLeft.reset();
    pidRight.reset();
}
