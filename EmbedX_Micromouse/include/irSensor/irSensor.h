#ifndef IRSENSOR_H

#define IRSENSOR_H

class irSensor{
    public:
        const int pin;
        int getObs();
        void setPin(int pin);
    private:
        int obs;
};

#endif