#include <stdio.h>
#include <stdarg.h>
#include "syscall.h"

int printf(const char * restrict fmt, ...)
{
    int ret;
    va_list ap;
    va_start(ap, fmt);
    ret = vdprintf(1, fmt, ap);
    va_end(ap);
    return ret;
}
