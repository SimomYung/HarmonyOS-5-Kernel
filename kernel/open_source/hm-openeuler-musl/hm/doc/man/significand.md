# significand(), significandf()
Get mantissa of floating-point number

## Synopsis
```c
#include <math.h>

double significand(double x);
float significandf(float x);.
```

## Arguments

- **x**
The number to be scaled.

## Library
libc

## Description
These functions return x mantissa scaled to the [1,2] range, the same to scalb(*x*, (double) -ilogb(*x*)).

## Returns

- **scalb(x, (double) -ilogb(x))**
If success.

## Errors

N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
