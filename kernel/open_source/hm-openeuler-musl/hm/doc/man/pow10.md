# pow10(), pow10f(), pow10l()
Base-10 power functions

## Synopsis
```c
#include <math.h>

double pow10(double x);
float pow10f(float x);
long double pow10l(long double x);
```

## Arguments
- **x**
The exponent number.

## Library
libc

## Description
These functions return 10 to the power of *x*.

## Returns

- **10 to the power of x**
If success.
&nbsp;

- **NaN**
If *x* is a NaN.
&nbsp;

- **positive infinity**
If *x* is positive infinity.
&nbsp;

- **+0**
If *x* is negative infinity.
&nbsp;

- **0**
If the result underflows.
&nbsp;

- **+HUGE_VAL/+HUGE_VALF/+HUGE_VALL**
If the result overflows.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
