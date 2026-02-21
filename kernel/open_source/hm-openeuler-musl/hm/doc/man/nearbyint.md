# nearbyint(), nearbyintf(), nearbyintl()
Round to nearest integer

## Synopsis
```c
#include <math.h>

double nearbyint(double x);
float nearbyintf(float x);
long double nearbyintl(long double x);
```

## Arguments
- **x**
The number to be rounded.

## Library
libc

## Description
These functions round *x* to the nearest floating-point integer value, the round direction can be changed using function fesetround().

## Returns
- **x**
If *x* is integral, +0, -0, NaN, or infinite.

## Errors
N/A.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
