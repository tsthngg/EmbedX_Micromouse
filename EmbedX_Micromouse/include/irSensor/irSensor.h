#ifndef IRSENSOR_H

#define IRSENSOR_H

class irSensor{
    public:
        irSensor(int pin);
        int checkObs();
    private:
        int pin;
};

int wallFront();
int wallLeft();
int wallRight();

#endif