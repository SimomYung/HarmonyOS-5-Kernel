# erf(), erff()
Error functions

## Synopsis

```c
#include <math.h>

double erf(double x);
float erff(float x);
```

## Arguments

- **x**
  The Floating-point number to be computed.


## Library
libc

## Description
These functions compute the error function of their argument x, defined as:
```
erf(x) = 2/sqrt(pi)* integral from 0 to x of exp(-t*t) dt.
```

## Returns

Upon successful completion, these functions return the value of the error function. These functions will not set errno currently.
&nbsp;
If *x* is a NaN, a NaN with the sign bit of *y* is returned.
&nbsp;
If *x* is +0 or -0, +0 or -0 is returned.
&nbsp;
If *x* is positive infinity or negative infinity, positive infinity or negative infinity is returned.
&nbsp;
If *x* is subnormal, a range error occurs, and ```2*x/sqrt(pi)``` should be returned.


## Errors

- **Range Error**
  When *x* is is less than 1 and exception FE_UNDERFLOW is raised.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety

TODO