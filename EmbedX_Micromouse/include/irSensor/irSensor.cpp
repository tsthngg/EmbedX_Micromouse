#include "irSensor.h"
#include <Arduino.h>

irSensor::irSensor(int pin) : pin(pin) {
    pinMode(pin, INPUT);
}

int irSensor::checkObs() {  //Kiểm tra chướng ngại vật
    if (digitalRead(pin) == HIGH) {
        return 1;
    }
    return 0;
}

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
