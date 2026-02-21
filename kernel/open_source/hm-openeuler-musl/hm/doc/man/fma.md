# fma(), fmaf(), fmal()
Floating-point multiply and add

## Synopsis

```c
#include <math.h>

double fma(double x, double y, double z);
float fmaf(float x, float y, float z);
long double fmal(long double x, long double y, long double z);
```

## Arguments

- **x**
  The Floating-point number that to be computed.
&nbsp;
- **y**
  The Floating-point number that to be computed.
&nbsp;
- **z**
  The Floating-point number that to be computed.

## Library
libc

## Description
These functions compute (*x* * *y*) + *z*.

## Returns

Upon successful completion, these functions return (*x* * *y*) + *z*, rounded as one ternary operation. These functions will not set errno currently.
&nbsp;
If *x* or *y* is NaN, a NaN is returned.
&nbsp;
If *x* multiplied by *y* is an exact infinity, and *z* is also an infinity but with the opposite sign, a NaN is returned.
&nbsp;
If one of *x* or *y* is infinite, the other is zero, and *z* is not a NaN, a NaN is returned.
&nbsp;
If one of *x* or *y* is infinite, and the other is zero, and *z* is a NaN, a NaN is returned.
&nbsp;
If *x* * *y* is not 0 * Inf nor Imf * 0 and *z* is a NaN, a NaN is returned.

## Errors

N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety

TODO