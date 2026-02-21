# cbrt(), cbrtf(), cbrtl()
Cube root functions

## Synopsis

```c
#include <math.h>

double cbrt(double x);
float cbrtf(float x);
long double cbrtl(long double x);
```

## Arguments

- **x**
  The Floating-point number to be computed.


## Library
libc

## Description
These functions are used to compute the real cube root of *x*.

## Returns

Upon successful completion, these functions return the real cube root of *x*.
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
