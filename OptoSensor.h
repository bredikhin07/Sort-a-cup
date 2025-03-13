#ifndef OPTOSENSOR_H
#define OPTOSENSOR_H

#include <Arduino.h>
#include "Logger.h"

class OptoSensor {
private:
    int pin;
    int whiteThreshold;
    bool inverted;
public:
    OptoSensor(int pin, int whiteThreshold);
    void init();
    int getRawValue();
    bool isTriggered();
    void setThreshold(int threshold);
    void setInversion(bool inv);
};

#endif // OPTOSENSOR_H
