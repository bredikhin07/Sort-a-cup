#ifndef GCODEPROCESSOR_H
#define GCODEPROCESSOR_H

#include <Arduino.h>
#include <GCodeParser.h>  // Предполагается наличие библиотеки GCodeParser
#include "StepperMotor.h"
#include "VibrMotor.h"
#include "OptoSensor.h"
#include "LEDController.h"
#include "Logger.h"
#include "AckModule.h"

class GCodeProcessor {
private:
    GCodeParser parser;
    StepperMotor* upperDisk;
    StepperMotor* lowerDisk;
    StepperMotor* capPicker;
    VibrMotor* vibrMotor;
    OptoSensor* optoUpper;
    OptoSensor* optoLower;
    OptoSensor* optoCap;
    LEDController* ledController;
    bool absoluteMode;
public:
    GCodeProcessor(StepperMotor* u, StepperMotor* l, StepperMotor* c,
                   VibrMotor* vm, OptoSensor* ou, OptoSensor* ol, OptoSensor* oc,
                   LEDController* led);
    void processLine();
    bool addChar(char c);
};

#endif // GCODEPROCESSOR_H
