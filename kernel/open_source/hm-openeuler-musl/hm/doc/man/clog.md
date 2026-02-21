#  clog(), clogf(), clogl()
Compute the complex natural logarithm of the given complex number

## Synopsis
```c
#include <complex.h>

double complex clog(double complex c);
float complex clogf(float complex c);
long double complex clogl(long double complex c);
```

## Arguments
- **c**
A complex number.

## Library
libc

## Description
These functions compute the complex natural logarithm of the given complex number *c*, and the branch cuts are along the negative real axis.

## Returns
The complex natural logarithm of the given complex number.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
