#  cpow(), cpowf(), cpowl()
Compute the power value of the given complex base and power

## Synopsis
```c
#include <complex.h>

double complex cpow(double complex c, complex double p);
float complex cpowf(float complex c, complex float p);
long double complex cpowl(long double complex c, complex long double p);
```

## Arguments
- **c**
A complex number as base.
&nbsp;
- **p**
A complex number as power.

## Library
libc

## Description
These functions compute the complex value *c* to the power of *p*, and the branch cuts are along the negative real axis.

## Returns
The power value of the given complex base and power.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
