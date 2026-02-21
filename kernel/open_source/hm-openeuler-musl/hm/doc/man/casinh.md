# casinh(), casinhf(), casinhl()

Complex arc hyperbolic sine

## Synopsis
```c
#include <complex.h>

double complex casinh(double complex z);
float complex casinhf(float complex z);
long double complex casinhl(long double complex z);
```

## Arguments

- **z**
Independent variable *z*.


## Library

libc

## Description

These functions calculate the complex arc hyperbolic sine of *z*. The imaginary part of the result is chosen in the interval [-pi/2, pi/2].

## Returns

The arc hyperbolic sine value of the given complex number.

## Errors

N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
