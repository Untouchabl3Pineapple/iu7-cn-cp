#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <stdio.h>
#include <stdarg.h>
#include <time.h>

typedef enum {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR
} LogLevel;

#define LOG_LEVEL   LOG_INFO
#define LOG_FILE    "../logs/server_log.txt"

void log_message(LogLevel level, const char* format, ...);

#endif // __LOGGER_H__
