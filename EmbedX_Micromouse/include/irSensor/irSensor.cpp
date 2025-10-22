#include "irSensor.h"
#include <Arduino.h>

int irSensor::getObs() {  //Kiểm tra chướng ngại vật
    if (digitalRead(pin) == HIGH) {
        return 1;
    }
    return 0;
}

void irSensor::setPin(int pin) { //Setup pinMode
    pinMode(pin, INPUT);
}

irSensor irFrontLeft;
irSensor irFrontRight;
irSensor irUpperLeft;
irSensor irLowerLeft;
irSensor irUpperRight;
irSensor irLowerRight;

int wallFront() {
    if (irFrontLeft.getObs() || irFrontRight.getObs()) {
        return 1;
    }
    return 0;
}

int wallLeft() {
    if (irUpperLeft.getObs() || irLowerLeft.getObs()) {
        return 1;
    }
    return 0;
}

int wallRight() {
    if (irUpperRight.getObs() || irLowerRight.getObs()) {
        return 1;
    }
    return 0;
}

void setup() {

}

void loop() {
    
}