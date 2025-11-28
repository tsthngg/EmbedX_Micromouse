#ifndef IRSENSOR_H

#define IRSENSOR_H

class irSensor{
    public:
        irSensor(int pin);
        int checkObs();
    private:
        int pin;
};

irSensor irFrontLeft(36);
irSensor irFrontRight(38);
irSensor irUpperLeft(42);
irSensor irLowerLeft(39);
irSensor irUpperRight(40);
irSensor irLowerRight(41);

int wallFront();
int wallLeft();
int wallRight();

#endif