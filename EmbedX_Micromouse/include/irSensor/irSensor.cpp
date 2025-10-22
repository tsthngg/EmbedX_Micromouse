#include "irSensor.h"
#include <Arduino.h>

int irSensor::getObs() {  //Checking obstacles
    if (digitalRead(pin) == HIGH) {
        obs = 1;
    }
    else {
        obs = 0;
    }
    return obs;
}

void irSensor::setPin(int pin) { //Setup pinMode
    pinMode(pin, INPUT);
}

int main() {
    return 0;
}