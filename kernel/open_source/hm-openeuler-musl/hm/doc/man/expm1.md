# expm1(), expm1f(), expm1l()
Exponential minus 1

## Synopsis
```c
#include <math.h>

double expm1(double x);
float expm1f(float x);
long double expm1l(long double x);
```

## Arguments

- **x**
The exponent number.

## Library
libc

## Description
These functions return e ^ *x* - 1.0.

## Returns

- **e ^ x - 1.0**
If success.
&nbsp;

- **NaN**
If *x* is a NaN.
&nbsp;

- **positive infinity**
If *x* is a positive infinity.
&nbsp;

- **-1**
If *x* is a negative infinity.
&nbsp;

- **+0/-0**
If *x* is +0/-0.
&nbsp;

- **-HUGE_VAL/-HUGE_VALF/-HUGE_VALL**
If the result overflows.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
