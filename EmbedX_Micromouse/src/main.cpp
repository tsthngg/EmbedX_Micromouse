#include "Motor_control.h"
#include <Arduino.h>
#include "BluetoothSerial.h"
BluetoothSerial SerialBT;

void setup(){
    setupMotor_control();
    Serial.begin(9600);
    SerialBT.begin("Motorcontrol");
      }

long long time_m = micros();
long long prevTime = 0;
long long setpoint;
int dirL = 1;
int dirR = 1;

void handleCommand(char currentState){
      switch (currentState){
        case 'F': {
          setpoint = goStraight(setpoint);
          dirL = 1;
          dirR = 1;
        }
          break;
        case 'L': {
          setpoint = turnLeft(setpoint);
          dirL = 0;
          dirR = 1;
        }
          break;
        case 'R': {
          setpoint = turnRight(setpoint);
          dirL = 1;
          dirR = 0;
        }
          break;
        case 'B' : turnBack();
          setpoint = turnBack();
          dirL = 1;
          dirR = 0;
      }
    }
    
          long long PWM_r = 0;
          long long PWM_l = 0;
          

void loop(){ 
      if(SerialBT.available()){
        char currentState = SerialBT.read();
        handleCommand(currentState);
      }

      if(setpoint != 0){

          unsigned long currentTime = micros();
          long long dt = currentTime - prevTime;
      
         PWM_r = Motor_controlLeft(setpoint, encoder1Value, dt);
         PWM_l = Motor_controlRight(setpoint, encoder2Value, dt);

          motor_control(PWM_r, PWM_l, dirL, dirR); // Gọi hàm điều khiển động cơ
          if((PWM_r < 50) || (PWM_l <50)){
            ledcWrite(0,0);
            ledcWrite(1,0);
            delay(800);
              PWM_r = 0;
              PWM_l = 0;
              encoder1Value = 0;
              encoder2Value = 0;
              setpoint = 0;
          }
      }
          Serial.print("Encoder 1 value : "); Serial.println(encoder1Value); 
          Serial.print("Encoder 2 Value : "); Serial.println(encoder2Value);
    }


 
