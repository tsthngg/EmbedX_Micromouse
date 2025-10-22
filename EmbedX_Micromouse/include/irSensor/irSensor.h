#ifndef IRSENSOR_H

#define IRSENSOR_H

class irSensor{
    public:
        const int pin;
        int getObs();
        void setPin(int pin);
};
int wallFront();
int wallLeft();
int wallRight();

#endif