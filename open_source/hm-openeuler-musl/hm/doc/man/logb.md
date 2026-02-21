# logb(), logbf(), logbl()
Get exponent of a floating-point value

## Synopsis
```c
#include <math.h>

double logb(double x);
float logbf(float x);
long double logbl(long double x);
```

## Arguments
- **x**
The number to be calculated to get the exponent.

## Library
libc

## Description
These functions return the logarithm of |*x*|, using FLT\_RADIX as base for logarithm. If FLT\_RADIX is 2, logb() is equal to log2() when *x* is positive.

## Returns
- **the exponent of x**
If success.
&nbsp;

- **NaN**
If *x* is a NaN.
&nbsp;

- **-HUGE_VAL/-HUGE_VALF/-HUGE_VALL**
If *x* is zero.
&nbsp;

- **positive infinity**
If *x* is a negative infinity or a positive infinity.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
