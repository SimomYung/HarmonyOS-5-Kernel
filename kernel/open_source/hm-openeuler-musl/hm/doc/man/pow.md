# pow(), powf(), powl()
Power functions

## Synopsis
```c
#include <math.h>

double pow(double x, double y);
float powf(float x, float y);
long double powl(long double x, long double y);
```

## Arguments
- **x**
The base number.
&nbsp;

- **y**
The exponent number.

## Library
libc

## Description
These functions return *x* to the power of *y*.

## Returns

- **x to the power of y**
If success.
&nbsp;

- **0.0**
If the result underflows and is not representable.
&nbsp;

- **NaN**
If *x* or *y* is a NaN.
If *x* is a finite value less than zero and *y* is a finite noninteger.
&nbsp;

- **1.0**
If *x* is +1(even if *y* is a NaN).
If *y* is 0(even if *x* is NaN).
If *x* is -1, and *y* is a infinity.
&nbsp;

- **+0/-0**
If *x* is +0/-0 and *y* is not an odd integer.
&nbsp;

- **+0**
If |*x*| != 1 and *y* is a infinity.
If *x* is a negative infinity and *y* < 0 and *y* is not an odd integer.
If *x* is a positive infinity and *y* < 0.
&nbsp;

- **-0**
If *x* is a negative infinity and *y* < 0 and *y* is an odd integer.
&nbsp;

- **positive infinity**
If |*x*| < 1 and *y* is a negative infinity.
If |*x*| > 1 and *y* is a positive infinity.
If *x* is a negative infinity and *y* > 0 and *y* is not an odd integer.
If *x* is a positive infinity and *y* > 0.
&nbsp;

- **negative infinity**
If *x* is a negative infinity and *y* > 0 and *y* is an odd integer.
&nbsp;

- **HUGE_VAL/HUGE_VALF/HUGE_VALL**
If *x* is +0/-0, *y* < 0 and *y* is an odd integer, the return value has the same sign with *x*.
If the result overflows.
&nbsp;

- **+HUGE_VAL/+HUGE_VALF/+HUGE_VALL**
If *x* is +0/-0, *y* < 0 and *y* is not an odd integer.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
