# sqrt(), sqrtf(), sqrtl()
Square root functions

## Synopsis
```c
#include <math.h>

double sqrt(double x);
float sqrtf(float x);
long double sqrtl(long double x);.
```

## Arguments

- **x**
The number to get it's non-negative square root.

## Library
libc

## Description
These functions return the non-negative square root of *x*.

## Returns

- **the square root of x**
If success.
&nbsp;

- **NaN**
If *x* is a NaN.
If *x* < 0.
&nbsp;

- **+0/-0**
If *x* is +0/-0.
&nbsp;

- **positive infinity**
If *x* is a positive infinity.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
