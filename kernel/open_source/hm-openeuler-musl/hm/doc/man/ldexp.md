# ldexp(), ldexpf(), ldexpl()
Floating-point number times 2 to the integer power

## Synopsis
```c
#include <math.h>

double ldexp(double x, int exp);
float ldexpf(float x, int exp);
long double ldexpl(long double x, int exp);
```

## Arguments
- **x**
A number.
&nbsp;
- **exp**
The exponent of 2.

## Library
libc

## Description
These functions return the result of floating point number *x* times 2 to the power *exp*.

## Returns
- **x * (2 ^ exp)**
If success.
&nbsp;

- **x**
If exp is 0.
&nbsp;

- **NaN**
If *x* is a NaN.
&nbsp;

- **infinity**
If *x* is an infinity, the same infinity will return.
&nbsp;

- **HUGE_VAL/HUGE_VALF/HUGE_VALL**
If the result overflows, and the return value will have the same sign of *x*.

## Errors
N/A

## Example
&nbsp;
## Classification
POSIX 1003.1-2017

## Function Safety
TODO
