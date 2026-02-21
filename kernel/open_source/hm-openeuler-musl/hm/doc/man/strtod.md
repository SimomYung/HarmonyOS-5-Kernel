# strtod(), strtof(), strtold()
Convert ASCII string to floating-point number

## Synopsis
```c
#include <stdlib.h>

double strtod(const char *s, char **p);
float strtof(const char *s, char **p);
long double strtold(const char *s, char **p);
```

## Arguments
- **s**
A string to be converted.
&nbsp;
- **p**
If *p* isn't null, this pointer stores the first unrecognized character found in the string.

## Library
libc

## Description
The strtod(), strtof(), and strtold() functions convert the initial portion of the string pointed to by *s* to double, float, and long double representation, respectively.

## Returns
These functions return the converted value on success.

If no conversion is performed, zero is returned and the value of *s* is stored in the location referenced by *p*.

If the correct value will cause an overflow, return positive and negative HUGE\_VAL according to the sign and set errno to ERANGE. If the correct value will cause an underflow, then zero is returned and errno is set to ERANGE.

## Errors
- **ERANGE**
Overflow or underflow occurred.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
