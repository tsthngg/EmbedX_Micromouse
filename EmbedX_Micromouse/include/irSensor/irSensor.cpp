#include "irSensor.h"
#include <Arduino.h>

irSensor::irSensor(int pin) {
    this->pin = pin;
    pinMode(pin, INPUT);
}

int irSensor::checkObs() {  //Kiểm tra chướng ngại vật
    if (digitalRead(pin) == HIGH) {
        return 1;
    }
    return 0;
}

irSensor irFrontLeft(36);
irSensor irFrontRight(37);
irSensor irUpperLeft(38);
irSensor irLowerLeft(39);
irSensor irUpperRight(40);
irSensor irLowerRight(41);

int wallFront() {
    if (irFrontLeft.checkObs() || irFrontRight.checkObs()) {
        return 1;
    }
    return 0;
}

int wallLeft() {
    if (irUpperLeft.checkObs() || irLowerLeft.checkObs()) {
        return 1;
    }
    return 0;
}

int wallRight() {
    if (irUpperRight.checkObs() || irLowerRight.checkObs()) {
        return 1;
    }
    return 0;
}

void setup() {
}
void loop() {  
}