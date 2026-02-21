# fabs(), fabsf(), fabsl()
Return the absolute value of a floating-point number

## Synopsis
```c
#include <math.h>

double fabs(double x);
float fabsf(float x);
long double fabsl(long double x);

Link with -lm.
```

## Arguments

- **x**
The variable.

## Library
libc

## Description
Return the absolute value of *x*.

## Returns

- **The absolute value of *x***
*x* is not a case as below.
&nbsp;
- **NAN**
*x* is NAN.
&nbsp;
- **+0**
*x* is -0.
&nbsp;
- **positive infinity**
*x* is negative infinity or positive infinity.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO