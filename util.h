#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

void log_message(const char* log_level, const char* msg, const char* type, ...);

#endif // UTIL_H
