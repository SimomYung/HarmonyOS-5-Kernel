#include <stdio.h>
#include <stdarg.h>
#include <limits.h>

int sprintf(char *restrict s, const char *restrict fmt, ...)
{
	int ret;
	va_list ap;
	va_start(ap, fmt);
	ret = vsprintf(s, fmt, ap);
	va_end(ap);
	return ret;
}

/* copy from glibc? just for libcstdc++ */
int __sprintf_chk(char *s, int flags, size_t len, const char *format, ...)
{
    va_list arg;
    int done;

    va_start (arg, format);

    if (len > (size_t) INT_MAX)
        done = vsprintf(s, format, arg);
    else
    {
        done = vsnprintf(s, len, format, arg);
        if (done >= 0 && (size_t) done >= len)
            return 0; // __chk_fail();
    }

    va_end (arg);

    return done;
}
