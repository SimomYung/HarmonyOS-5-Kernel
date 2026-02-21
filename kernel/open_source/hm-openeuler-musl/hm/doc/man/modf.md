# modf(), modff(), modfl()
Decompose floating-point number functions

## Synopsis

```c
#include <math.h>

double modf(double x, double *iptr);
float modff(float x, float *iptr);
long double modfl(long double x, long double *iptr);
```

## Arguments

- **x**
  floating point value.
&nbsp;
- **iptr**
  pointer to a double point(for modf()), or a float point(for modff()), or a long double(for modfl()) value to store the integral part to.

## Library
libc

## Description
These functions return the fractional part of *x* with the same sign as *x*. The integral part is put into the value pointed to by *iptr*.

## Returns

Upon successful completion, these functions return the signed fractional part of *x*.
&nbsp;
If *x* is NaN, a NaN is returned, and **iptr* is set to a NaN.
&nbsp;
If *x* is positive infinity (negative infinity), +0 (-0) is returned, and **iptr* is set to positive infinity (negative infinity).


## Errors

N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety

TODO
