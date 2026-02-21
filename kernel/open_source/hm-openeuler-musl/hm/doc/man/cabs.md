# cabs(), cabsf(), cabsl()
Compute the absolute value of the given complex number

## Synopsis
```c
#include <complex.h>

double cabs(double complex c);
float cabsf(float complex c);
long double cabsl(long double complex c);
```

## Arguments
- **c**
A complex number.

## Library
libc

## Description
These functions compute the absolute value of the given complex number *c*. Assuming that *c* is of format x \+ I \* y, then the result is equal to:
```
sqrt(x*x + y*y)
```

## Returns
The absolute value of the given complex number.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
