# fmod(), fmodf(), fmodl()
Floating-point remainder value functions

## Synopsis

```c
#include <math.h>

double fmod(double x, double y);
float fmodf(float x, float y);
long double fmodl(long double x, long double y);
```

## Arguments

- **x**
  divisor.
&nbsp;
- **y**
  dividend.

## Library
libc

## Description
These functions return floating-point remainder of the division of *x* by *y*.

## Returns

Upon successful completion, these functions return the lesser value of *x* and *y*. These functions will not set errno currently.
&nbsp;
If *x* or *y* is NaN, NaN is returned.
&nbsp;
If *x* is infinity, NaN is returned.
&nbsp;
If *y* is zero, NaN is returned.
&nbsp;
If *x* is +0 and -0, and *y* is not zero, +0 and -0 is returned.

## Errors

N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety

TODO