#include "motorControl.h"
#include <Arduino.h>

// Định nghĩa chân động cơ
// Khai báo chân encoder (tiến độ hiện tại chưa dùng tới)
const int leftEncoder = 34;
const int rightEncoder = 35;

// Chân động cơ trái
const int motorPWM_L = 25;
const int motorDir1_L = 26;
const int motorDir2_L = 27;

// Chân động cơ phải
const int motorPWM_R = 14;
const int motorDir1_R = 12;
const int motorDir2_R = 13;

void motorSetup() {
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

void forward(int speed) {
  digitalWrite(motorDir1_L, HIGH);
  digitalWrite(motorDir2_L, LOW);
  ledcWrite(0, speed);

  digitalWrite(motorDir1_R, HIGH);
  digitalWrite(motorDir2_R, LOW);
  ledcWrite(1, speed);
}

void turnLeft(int speed) {
  digitalWrite(motorDir1_L, LOW);
  digitalWrite(motorDir2_L, HIGH);
  ledcWrite(0, speed);

  digitalWrite(motorDir1_R, HIGH);
  digitalWrite(motorDir2_R, LOW);
  ledcWrite(1, speed);
}

void turnRight(int speed) {
  digitalWrite(motorDir1_L, HIGH);
  digitalWrite(motorDir2_L, LOW);
  ledcWrite(0, speed);

  digitalWrite(motorDir1_R, LOW);
  digitalWrite(motorDir2_R, HIGH);
  ledcWrite(1, speed);
}

void stop() {
  ledcWrite(0, 0);
  ledcWrite(1, 0);
}

