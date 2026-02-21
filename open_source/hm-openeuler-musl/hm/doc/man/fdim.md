# fdim(), fdimf(), fdiml()
Compute positive difference between two floating-point numbers

## Synopsis

```c
#include <math.h>

double fdim(double x, double y);
float fdimf(float x, float y);
long double fdiml(long double x, long double y);
```

## Arguments

- **x**
  The Floating-point number to be computed.
&nbsp;
- **y**
  The Floating-point number to be computed.

## Library
libc

## Description
These functions compute positive difference between two floating-point numbers.

## Returns

Upon successful completion, these functions return the positive difference between *x* and *y*. These functions will not set errno currently.
&nbsp;
If *x* or *y* is a NaN, a NaN is returned.


## Errors

N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety

TODO