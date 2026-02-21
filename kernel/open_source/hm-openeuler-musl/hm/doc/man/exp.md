# exp(), expf(), expl()
Base-e exponential functions

## Synopsis
```c
#include <math.h>

double exp(double x);
float expf(float x);
long double expl(long double x);
```

## Arguments

- **x**
The exponent number.

## Library
libc

## Description
These functions return e ^ *x*.

## Returns

- **e ^ x**
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
