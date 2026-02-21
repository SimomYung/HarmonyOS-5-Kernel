# remainder(), remainderl()
Floating-point remainder functions

## Synopsis
```c
#include <math.h>

double remainder(double x, double y);
long double remainderl(long double x, long double y);
```

## Arguments

- **x**
The number to be divided.
&nbsp;

- **y**
The number to divide.

## Library
libc

## Description
These functions return the remainder of *x* divided by *y*. When *y* / *x* = n, and n is rounded to the nearest integer, these functions return *y* - n * *x*. If |*x* - n * *y*| is 0.5, n is chosen to be even.

## Returns
- **the floating-point remainder**
If success.
&nbsp;

- **NaN**
If *x* ia a NaN.
If *x* is an infinity and *y* is not a NaN.
If *y* = 0 and *x* is not a NaN.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
