#include "VibrMotor.h"

VibrMotor::VibrMotor(int pin, int defaultSpeed)
    : pin(pin), speed(defaultSpeed), state(false) {
}

void VibrMotor::init() {
    LOG_INFO("Initializing VibrMotor on pin " + String(pin));
    pinMode(pin, OUTPUT);
}

void VibrMotor::setSpeed(int newSpeed) {
    speed = newSpeed;
    analogWrite(pin, state ? speed : 0);
    LOG_INFO("VibrMotor speed set to " + String(speed));
}

void VibrMotor::turn(bool newState) {
    state = newState;
    analogWrite(pin, state ? speed : 0);
    LOG_INFO("VibrMotor turned " + String(state ? "ON" : "OFF"));
}

void VibrMotor::turnOn() {
    turn(true);
}

void VibrMotor::turnOff() {
    turn(false);
}
