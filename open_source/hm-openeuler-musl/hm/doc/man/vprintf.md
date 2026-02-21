# vprintf(), vfprintf(), vsnprintf(), vsprintf()
Formatted output conversion

## Synopsis
```c
#include <stdarg.h>

int vprintf(const char *fmt, va_list ap);
int vfprintf(FILE *f, const char *fmt, va_list ap);
int vsnprintf(char *s, size_t size, const char *fmt, va_list ap);
int vsprintf(char *s, const char *fmt, va_list ap);
```

## Arguments

- **fmt**
A character string which is similar to the first argument of printf().
&nbsp;
- **ap**
A va\_list composed of many arguments.
&nbsp;
- **s**
The target string to format the arguments.
&nbsp;
- **f**
The target stream to format the arguments.
&nbsp;
- **size**
The length of the argument *s*, this argument is checked for not overflowing the actual space.

## Library
libc

## Description
These functions are similar to format arguments to target string or stream. Callers must be careful not to overflow the actual space.

## Returns
On success, these functions return the number of characters printed (excluding the null byte used to end output to strings). On error, a negative value is returned.

## Errors

N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
