# strtod\_l(), strtof\_l(), strtold\_l()
Convert a string into a double-precision number

## Synopsis
```c
#define _GNU_SOURCE
#include <stdlib.h>

double strtod_l(const char * s, char ** p, struct __locale_struct loc);

float strtof_l(const char * s, char ** p, struct __locale_struct loc);

long double strtold_l(const char * s, char ** p, struct __locale_struct loc);
```

## Arguments
- **s**
A string to be converted.
&nbsp;
- **p**
If *p* isn't null, this pointer stores the first unrecognized character found in the string.
&nbsp;
- **loc**
Used to specified locale rather than the global or pre-thread locale.
This argument has no effect for now.

## Library
libc

## Description
These functions return the converted value on success.

## Returns
These functions return the converted value on success.

If the correct value will cause an overflow, return positive and negative HUGE\_VAL according to the sign and set errno to ERANGE. If the correct value will cause an underflow, then zero is returned and errno is set to ERANGE.

## Errors
- **ERANGE**
The value to be returned would cause overflow or underflow.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
