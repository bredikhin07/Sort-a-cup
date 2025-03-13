#ifndef STEPPERMOTOR_H
#define STEPPERMOTOR_H

#include <Arduino.h>
#include <ESP_FlexyStepper.h>
#include "Logger.h"

class StepperMotor {
private:
    ESP_FlexyStepper stepper;
    int stepPin, dirPin;
    float stepsPerRevolution;
    int homeDirection;
    float lastTarget; // Текущая цель в градусах
public:
    StepperMotor(int stepPin, int dirPin, float stepsPerRev, int homeDirection);
    void init();
    void moveAbsolute(float targetDeg);
    void moveRelative(float deltaDeg);
    float getPosition(); // Возвращает позицию в градусах
    void setOrigin();
    bool isMoving();
    void stop();
    void release();
    void lock();
    // Новые методы для изменения скорости и ускорения (в градусах)
    void setSpeed(float speedDegPerSec);
    void setAcceleration(float accelDegPerSec2);
};

#endif // STEPPERMOTOR_H
