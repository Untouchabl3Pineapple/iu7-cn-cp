#include "logger.h"

void log_message(LogLevel level, const char* format, ...) {
    time_t rawtime;
    struct tm* timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    LogLevel current_log_level = LOG_LEVEL;

    if (level > current_log_level) { return; }

    FILE* log_file = fopen(LOG_FILE, "a");
    if (log_file == NULL) {
        perror("Error opening log file");
        return;
    }

    char time_str[50];
    strftime(time_str, sizeof(time_str), "[%Y-%m-%d %H:%M:%S]", timeinfo);

    va_list args;
    va_start(args, format);

    fprintf(log_file, "%s ", time_str);

    switch (level) {
    case LOG_DEBUG:
        fprintf(log_file, "[DEBUG] ");
        break;
    case LOG_INFO:
        fprintf(log_file, "[INFO] ");
        break;
    case LOG_WARNING:
        fprintf(log_file, "[WARNING] ");
        break;
    case LOG_ERROR:
        fprintf(log_file, "[ERROR] ");
        break;
    }

    vfprintf(log_file, format, args);

    va_end(args);
    fclose(log_file);
}
