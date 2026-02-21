# cacosh(), cacoshf(), cacoshl()

Complex arc hyperbolic cosine

## Synopsis
```c
#include <complex.h>

double complex cacosh(double complex z);
float complex cacoshf(float complex z);
long double complex cacoshl(long double complex z);
```

## Arguments

- **z**
Independent variable *z*.

## Library

libc

## Description

These functions calculate the complex arc hyperbolic cosine of *z*. The imaginary part of the result is chosen in the interval [-pi, pi], and the real part of the result is chosen non-negative.

## Returns

The arc hyperbolic cosine value of the given complex number.

## Errors

N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
