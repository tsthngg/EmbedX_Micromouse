#include "Motor_control.h"

// Định nghĩa chân động cơ
// Khai báo chân encoder (tiến độ hiện tại chưa dùng tới)
const int encoder1_A = 34;
const int encoder1_B = 35;

const int encoder_2A = 32;
const int encoder_2B = 33;

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


void setupMotor_control() {
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

void di_thang() {
  digitalWrite(motorDir1_L, LOW);
  digitalWrite(motorDir2_L, HIGH);
  ledcWrite(0,255);

  digitalWrite(motorDir1_R, HIGH);
  digitalWrite(motorDir2_R, LOW);
  ledcWrite(1, 255);

  delay(3000);
}

void re_trai() {
  digitalWrite(motorDir1_L, LOW);
  digitalWrite(motorDir2_L, HIGH);
  ledcWrite(0,180);

  digitalWrite(motorDir1_R, LOW);
  digitalWrite(motorDir2_R, HIGH);
  ledcWrite(1,180);

  delay(650);
}

void re_phai() {
  digitalWrite(motorDir1_L, HIGH);
  digitalWrite(motorDir2_L, LOW);
  ledcWrite(0, 180);

  digitalWrite(motorDir1_R, HIGH);
  digitalWrite(motorDir2_R, LOW);
  ledcWrite(1, 180);

  delay(650);
}

void dung_lai() {
  ledcWrite(0, 0);
  ledcWrite(1, 0);

  delay(2000);
}

void quay_dau(){
  digitalWrite(motorDir1_L, HIGH);
  digitalWrite(motorDir2_L, LOW);
  ledcWrite(0, 180);

  digitalWrite(motorDir1_R, HIGH);
  digitalWrite(motorDir2_R, LOW);
  ledcWrite(1, 180);

  delay(1300);


}
