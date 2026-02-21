# exp2(), exp2f(), exp2l()
Base-2 exponential functions

## Synopsis
```c
#include <math.h>

double exp2(double x);
float exp2f(float x);
long double exp2l(long double x);
```

## Arguments

- **x**
The exponent number.

## Library
libc

## Description
These functions return 2 ^ *x*.

## Returns

- **2 ^ x**
If success.
&nbsp;

- **NaN**
If *x* is a NaN.
&nbsp;

- **positive infinity**
If *x* is a positive infinity.
&nbsp;

- **+0**
If *x* is a negative infinity.
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
