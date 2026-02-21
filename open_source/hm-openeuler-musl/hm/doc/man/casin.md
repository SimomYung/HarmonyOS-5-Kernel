# casin(), casinl()

Complex arc sine

## Synopsis
```c
#include <complex.h>

double complex casin(double complex z);
long double complex casinl(long double complex z);
```

## Arguments

- **z**
Independent variable *z*.

## Library

libc

## Description

These functions calculate the complex arc sine of *z*. The real part of the result is chosen in the interval [-pi/2, pi/2].

## Returns

The arc sine value of the given complex number.

## Errors

N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
