#  nextafter(), nextafterf(), nextafterl()
Floating-point number manipulation

## Synopsis
```c
#include <math.h>

double nextafter(double x, double y);
float nextafterf(float x, float y);
long double nextafterl(long double x, long double y);
```

## Arguments
- **x**
The number to be rounded.
&nbsp;
- **y**
The number *x* is rounded in the direction of *y*.

## Library
libc

## Description
In the direction of *y*, these functions return the next representable floating-point value following *x*.
&nbsp;
If *x* == *y*, these functions return *y*.
&nbsp;
If *x* > *y*, these functions return the largest representable number less than *x*;
&nbsp;
The difference between nexttoward() functions and nextafter() functions is that the nexttoward() functions have a long double second argument.

## Returns
- **the next representable floating-point value after x in the direction of y**
If success.
&nbsp;

- **NaN**
If *x* or *y* is a NaN.
&nbsp;

- **HUGE_VAL/HUGE_VALF/HUGE_VALL**
If *x* is finite.
&nbsp;

- **0.0**
If the correct result is zero, subnormal or underflow and *x* is equal to *y*. In this case, whether the correct result or 0.0 is returned.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
