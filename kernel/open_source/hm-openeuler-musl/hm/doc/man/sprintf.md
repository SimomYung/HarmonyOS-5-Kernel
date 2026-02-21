# sprintf(), snprintf()
Input format conversion

## Synopsis
```c
#include <stdio.h>

int sprintf(char *restrict s, const char *restrict fmt, ...);
int snprintf(char *restrict s, size_t n, const char *restrict fmt, ...);
```

## Arguments
- **fmt**
A sequence of characters to describe how subsequent arguments are converted for output.
&nbsp;
- **s**
The character string used to write to.
&nbsp;
- **n**
The maximum number of the bytes to be written.
&nbsp;
- **...**
A variable number of arguments according to *fmt*.

## Library
libc

## Description
The format string is a character string, is same to the first argument of printf(). These functions write the output under the control of the format string that specifies how subsequent arguments are converted for output. The functions snprintf() write at most *n* bytes (including the terminating null byte ('\0')) to string *s*. Callers must be careful not to overflow the actual space of string *s*.
&nbsp;
## Returns
On success, these functions return the number of characters written to the final string *s*. On error, a negative value is returned.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
