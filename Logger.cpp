#include "Logger.h"

LogLevel Logger::currentLogLevel = LOG_DEBUG;

void Logger::init() {
    currentLogLevel = LOG_DEBUG;
}

void Logger::setLogLevel(LogLevel level) {
    currentLogLevel = level;
    Serial.println("[LOGGER] Log level set to " + String((int)level));
}

LogLevel Logger::getLogLevel() {
    return currentLogLevel;
}

void Logger::log(LogLevel level, const String &msg) {
    if (level <= currentLogLevel && currentLogLevel != LOG_OFF) {
        String prefix;
        switch(level) {
            case LOG_ERROR: prefix = "[ERROR] "; break;
            case LOG_WARN:  prefix = "[WARN] ";  break;
            case LOG_INFO:  prefix = "[INFO] ";  break;
            case LOG_DEBUG: prefix = "[DEBUG] "; break;
            default: break;
        }
        Serial.println(prefix + msg);
    }
}
