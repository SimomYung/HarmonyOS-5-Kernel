# vasprintf()
Formatted output conversion

## Synopsis
```c
#define _GNU_SOURCE
#include <stdio.h>

int vasprintf(char **s, const char *fmt, va_list ap);
```

## Arguments

- **s**
A pointer points a address which is allocated by this function to hold the output including the terminating null byte ('\0').
&nbsp;
- **fmt**
A sequence of characters to describe how subsequent arguments are converted for output.
&nbsp;
- **ap**
A va_list composed of many arguments.

## Library
libc

## Description
vasprintf() allocates a buf large enough to hold the final string and return a pointer to it via the first argument. This pointer should be freed with free().

## Returns
On success, these functions return the number of characters written to the final string. On error, -1 is returned.

## Errors

N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
