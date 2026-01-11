#include "irSensor/irSensor.h"
#include <Arduino.h>

bool checkWall(char side) {
    if (side == 'l') {
        int pin1 = irLowerLeftPin;
        int pin2 = irUpperLeftPin;
    }
    else if (side == 'r') {
        int pin1 = irLowerRightPin;
        int pin2 = irUpperRightPin;
    }
    else if (side == 'f') {
        int pin1 = irFrontLeftPin;
        int pin2 = irFrontRightPin;
    }
    else {
        return false; // Invalid side
    }
    if (digitalread(pin1) == LOW || digitalread(pin2) == LOW) {
        return true;
    } 
    else { 
        return false;
    }
}

void irSensorSetup() {
    pinMode(irFrontLeftPin, INPUT);
    pinMode(irFrontRightPin, INPUT);
    pinMode(irUpperLeftPin, INPUT);
    pinMode(irUpperRightPin, INPUT);
    pinMode(irLowerLeftPin, INPUT);
    pinMode(irLowerRightPin, INPUT);
}

