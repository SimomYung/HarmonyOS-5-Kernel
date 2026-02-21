# rint(), rintf(), rintl()
Round to nearest integer

## Synopsis
```c
#include <math.h>

double rint(double x);
float rintf(float x);
long double rintl(long double x);
```

## Arguments
- **x**
The number to be rounded.

## Library
libc

## Description
These functions round *x* to the nearest floating-point integer value, the round direction can be changed using function fesetround(). If the result is different from the argument exception FE\_INEXACT is raised.

## Returns
- **the rounded integer value**
If success.
&nbsp;

- **x**
If *x* is integral, +0, -0, NaN, or infinite.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
