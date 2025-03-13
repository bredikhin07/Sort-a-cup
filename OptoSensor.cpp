#include "OptoSensor.h"

OptoSensor::OptoSensor(int pin, int whiteThreshold)
    : pin(pin), whiteThreshold(whiteThreshold), inverted(false) {
}

void OptoSensor::init() {
    LOG_INFO("Initializing OptoSensor on pin " + String(pin));
    pinMode(pin, INPUT);
}

int OptoSensor::getRawValue() {
    int value = analogRead(pin);
    LOG_DEBUG("OptoSensor on pin " + String(pin) + ": raw=" + String(value));
    return value;
}

bool OptoSensor::isTriggered() {
    int raw = getRawValue();
    bool triggered = (raw >= whiteThreshold);
    if (inverted) {
        triggered = !triggered;
    }
    LOG_DEBUG("OptoSensor on pin " + String(pin) + ": triggered=" + String(triggered));
    return triggered;
}

void OptoSensor::setThreshold(int threshold) {
    whiteThreshold = threshold;
    LOG_INFO("OptoSensor on pin " + String(pin) + " threshold set to " + String(threshold));
}

void OptoSensor::setInversion(bool inv) {
    inverted = inv;
    LOG_INFO("OptoSensor on pin " + String(pin) + " inversion set to " + String(inv ? "ON" : "OFF"));
}
