# ceil(), ceilf(), ceill()
Ceiling value functions

## Synopsis

```c
#include <math.h>

double ceil(double x);
float ceilf(float x);
long double ceill(long double x);
```

## Arguments

- **x**
  The Floating-point number to be computed.


## Library
libc

## Description
These functions are used to compute the smallest integral value not less than *x*.

## Returns

Upon successful completion, these functions return the smallest integral value not less than *x*.
&nbsp;
If *x* is NaN, a NaN is returned.
&nbsp;
If *x* is +0 or -0, +0 or -0 is returned.
&nbsp;
If x is positive infinity or negative infinity, positive infinity or negative infinity is returned.


## Errors

N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety

TODO