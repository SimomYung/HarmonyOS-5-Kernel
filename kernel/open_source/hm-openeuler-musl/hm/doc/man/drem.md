# drem(), dremf()
Floating-point remainder functions

## Synopsis
```c
#include <math.h>

double drem(double x, double y);
float dremf(float x, float y);
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
These functions return the remainder of *x* divided by *y*. When *y* / *x* = n, and n is rounded to the nearest integer, these functions return *y* - n * *x*. If |*x* - n * *y*| is 0.5, n is chosen to be even. These functions will not set errno currently.

## Returns
- **the floating-point remainder**
Upon successful completion, these functions return the floating-point remainder.
&nbsp;

- **NaN**
If *x* ia a NaN.
&nbsp;
If *x* is an infinity and *y* is not a NaN.
&nbsp;
If *y* is 0 and *x* is not a NaN.

## Errors

N/A

## Example
&nbsp;

## Classification
Unix

## Function Safety
TODO