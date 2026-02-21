# fmin(), fminf(), fminl()
Determine minimum numeric value of two floating-point numbers

## Synopsis

```c
#include <math.h>

double fmin(double x, double y);
float fminf(float x, float y);
long double fminl(long double x, long double y);
```

## Arguments

- **x**
  The Floating-point number that to be compared.
&nbsp;
- **y**
  The Floating-point number that to be compared.

## Library
libc

## Description
These functions return the minimum of *x* and *y*.

## Returns

Upon successful completion, these functions return the lesser value of *x* and *y*.
&nbsp;
If *x* or *y* is NaN, the other argument is returned.
&nbsp;
If *x* and *y* are NaN, NaN is returned.

## Errors

N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety

TODO