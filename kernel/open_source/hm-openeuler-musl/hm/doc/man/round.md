# round(), roundf(), roundl()
These functions round to the nearest integer, away from zero

## Synopsis
```c
#include <math.h>

double round(double x);
float roundf(float x);
long double roundl(long double x);
```

## Arguments

- **x**
The number to be rounded.

## Library
libc

## Description
These functions round *x* to the nearest integer but round it to half of zero.
&nbsp;
These functions will not set floating-point status flag, because the relevant instructions can not generate floating-point exception results.

## Returns

- **the rounded integer value**
If success.
&nbsp;

- **x**
If *x* is integral, +0, -0, NaN, or an infinite.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
