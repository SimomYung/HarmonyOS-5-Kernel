# erfc(), erfcf()
Complementary error functions

## Synopsis
```c
#include <math.h>

double erfc(double x);
float erfcf(float x);
```

## Arguments

- **x**
  The Floating-point number to be computed.

## Library
libc

## Description
These functions return the value of the complementary error function.

## Returns

Upon successful completion, these functions return the value of the complementary error function. These functions will not set errno currently.
&nbsp;
If *x* is a NaN, a NaN with the sign bit of *y* is returned.
&nbsp;
If *x* is +0 or -0, +1 is returned.
&nbsp;
If *x* is negative infinity, +2 is returned.
&nbsp;
If *x* is positive infinity, +0 is returned.
&nbsp;
If these functions result underflows and produces an unrepresentable value, the return value is 0.0.


## Errors

- **Range Error**
  When result is subnormal and exception FE_UNDERFLOW is raised.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety

TODO