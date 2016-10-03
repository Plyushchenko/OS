#ifndef __IO_H_
#define __IO_H_

#include <stdint.h>
#include <stdarg.h>
#include <stddef.h>
#include <ioport.h>

int printf(const char* format, ...);
int vprintf(const char* fmt, va_list args);
int snprintf (char* s, size_t n, const char* format, ...);
int vsnprintf(char* s, size_t n, const char* format, va_list args);

#endif /* IO_H */

