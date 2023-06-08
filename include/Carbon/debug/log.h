#ifndef _LOG_H_
#define _LOG_H_

#include <stdarg.h>

void log_show_system(int en);
void log_show_debug(int en);
void log_show_warning(int en);
void log_show_error(int en);

// Print a Custom Log
void log_vcustom(char type, const char *tag, const char *message, va_list args);

// Print a Custom Log
void log_custom(char type, const char *tag, const char *message, ...);

// Print a System Message
void log_system(const char *tag, const char *message, ...);

// Print a Debug Message
void log_debug(const char *tag, const char *message, ...);

// Print a Warning Message
void log_warning(const char *tag, const char *message, ...);

// Print an Error Message
void log_error(const char *tag, const char *message, ...);

#endif