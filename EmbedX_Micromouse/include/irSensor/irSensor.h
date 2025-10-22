#ifndef IRSENSOR_H

#define IRSENSOR_H

class irSensor{
    public:
        const int pin;
        int getObs();
        void setPin(int pin);
};

irSensor irFrontLeft;
irSensor irFrontRight;
irSensor irUpperLeft;
irSensor irLowerLeft;
irSensor irUpperRight;
irSensor irLowerRight;

int wallFront();
int wallLeft();
int wallRight();

#endif