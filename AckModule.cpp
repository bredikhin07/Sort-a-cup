#include "AckModule.h"

void AckModule::sendAck(const String &id, const String &status) {
    if (id.length() > 0) {
        Serial.println("[ACK] " + id + " " + status);
    } else {
        Serial.println("[ACK] " + status);
    }
}
