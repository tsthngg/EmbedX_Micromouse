#ifndef IRSENSOR_H

#define IRSENSOR_H

class irSensor{
    public:
        irSensor(int pin);
        int checkObs();
    private:
        int pin;
};

extern irSensor irFrontLeft(36);
extern irSensor irFrontRight(38);
extern irSensor irUpperLeft(42);
extern irSensor irLowerLeft(39);
extern irSensor irUpperRight(40);
extern irSensor irLowerRight(41);

extern int wallFront();
extern int wallLeft();
extern int wallRight();

#endif