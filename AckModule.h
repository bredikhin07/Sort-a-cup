#ifndef ACKMODULE_H
#define ACKMODULE_H

#include <Arduino.h>

class AckModule {
public:
    static void sendAck(const String &id, const String &status);
};

#endif // ACKMODULE_H
