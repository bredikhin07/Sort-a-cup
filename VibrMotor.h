#ifndef VIBRMOTOR_H
#define VIBRMOTOR_H

#include <Arduino.h>
#include "Logger.h"

class VibrMotor {
private:
    int pin;
    int speed;
    bool state;
public:
    VibrMotor(int pin, int defaultSpeed = 255);
    void init();
    void setSpeed(int newSpeed);
    void turn(bool newState);
    void turnOn();
    void turnOff();
};

#endif // VIBRMOTOR_H
