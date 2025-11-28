#include "Motor_control.h"
#include <Arduino.h>


  void setup(){
    setupMotor_control();
    Serial.begin(9600);
    }

  void loop(){
    goStraight(1060,1060);
    delay(2000);
    turnLeft(550,550);
    delay(2000);
    turnRight(550,550);
    delay(2000);
    Serial.print("Gia tri encoder 1 la :"); Serial.println(encoder1Value);
    Serial.print("Gia tri encoder 2 la :"); Serial.println(encoder2Value);


   // di_thang();
   // quay_dau();
   //re_trai();
   //di_thang();
   //quay_dau();
   //di_thang();
  }
