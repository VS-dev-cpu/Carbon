#include <Carbon/debug/log.h>

#include <stdio.h>
#include <string.h>
#include <time.h>

// Get Time as String
void stime(char *str) {
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    sprintf(str, "%d.%d.%d %d:%d:%d", timeinfo->tm_year + 1900,
            timeinfo->tm_mon + 1, timeinfo->tm_mday, timeinfo->tm_hour,
            timeinfo->tm_min, timeinfo->tm_sec);
}

void log_vcustom(char type, const char *tag, const char *message,
                 va_list args) {
    // TODO: tag cutting

    char msg[1024];
    char date[32];
    stime(date);

    sprintf(msg, "[%s] %s [%c]  %s\n", date, tag, type, message);

    // Print Out
    vprintf(msg, args);

    // TODO: Write to File
}

void log_custom(char type, const char *tag, const char *message, ...) {
    va_list args;
    va_start(args, message);

    log_vcustom(type, tag, message, args);

    va_end(args);
}

void log_system(const char *tag, const char *message, ...) {
    va_list args;
    va_start(args, message);

    if (log_enable_system)
        log_vcustom('S', tag, message, args);

    va_end(args);
}

void log_debug(const char *tag, const char *message, ...) {
    va_list args;
    va_start(args, message);

    if (log_enable_debug)
        log_vcustom('D', tag, message, args);

    va_end(args);
}

void log_warning(const char *tag, const char *message, ...) {
    va_list args;
    va_start(args, message);

    if (log_enable_warning)
        log_vcustom('W', tag, message, args);

    va_end(args);
}

void log_error(const char *tag, const char *message, ...) {
    va_list args;
    va_start(args, message);

    if (log_enable_error)
        log_vcustom('E', tag, message, args);

    va_end(args);
}