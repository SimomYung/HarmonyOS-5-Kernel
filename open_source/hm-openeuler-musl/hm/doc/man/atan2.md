# atan2(), atan2f(), atan2l()

Arc tangent function of two variables

## Synopsis

```c
#include <math.h>

double atan2(double y, double x);
float atan2f(float y, float x);
long double atan2l(long double y, long double x);
```

## Arguments 

- **x**
Independent variable *x*.
&nbsp;
- **y**
Independent variable *y*.

## Library

libc

## Description

These functions calculate the principal value of the arc tangent of *y*/*x*, using the signs of the two arguments to determine the quadrant of the result.

## Returns

- **[-pi, pi]**
On success, these functions return the principal value of the arc tangent of *y*/*x* in radians; the return value is in the range [-pi, pi].
If *y* is a finite value greater (less) than 0, and *x* is negative infinity, +pi (-pi) is returned.
If *y* is a finite value greater (less) than 0, and *x* is positive infinity, +0 (-0) is returned.
If *y* is positive infinity (negative infinity), and *x* is finite, pi/2 (-pi/2) is returned.
If *y* is positive infinity (negative infinity) and *x* is negative infinity, +3*pi/4 (-3*pi/4) is returned.
If *y* is positive infinity (negative infinity) and *x* is positive infinity, +pi/4 (-pi/4) is returned.
&nbsp;
- **NaN**
If either *x* or *y* is NaN, a NaN is returned.

## Errors

N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO