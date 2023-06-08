#ifndef _LOG_H_
#define _LOG_H_

#include <stdarg.h>

int log_enable_system = 1;
int log_enable_debug = 1;
int log_enable_warning = 1;
int log_enable_error = 1;

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