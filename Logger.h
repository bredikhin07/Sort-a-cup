#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>

enum LogLevel {
    LOG_OFF = 0,
    LOG_ERROR = 1,
    LOG_WARN  = 2,
    LOG_INFO  = 3,
    LOG_DEBUG = 4
};

class Logger {
public:
    static void init(); // Инициализация (например, установка дефолтного уровня)
    static void setLogLevel(LogLevel level);
    static LogLevel getLogLevel();
    static void log(LogLevel level, const String &msg);
private:
    static LogLevel currentLogLevel;
};

#define __LOGGER_ENABLE__

#ifdef __LOGGER_ENABLE__
	#define LOG_ERROR(msg) Logger::log(LOG_ERROR, msg)
	#define LOG_WARN(msg)  Logger::log(LOG_WARN, msg)
	#define LOG_INFO(msg)  Logger::log(LOG_INFO, msg)
	#define LOG_DEBUG(msg) Logger::log(LOG_DEBUG, msg)
#else
	#define LOG_ERROR(msg)
	#define LOG_WARN(msg)
	#define LOG_INFO(msg)
	#define LOG_DEBUG(msg)
#endif //__LOGGER_ENABLE__

#endif // LOGGER_H
