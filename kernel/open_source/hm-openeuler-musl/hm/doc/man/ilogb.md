# ilogb(), ilogbf(), ilogbl()
Get the exponent portion of a floating-point number

## Synopsis
```c
#include <math.h>

int ilogb(double x);
int ilogbf(float x);
int ilogbl(long double x);
```

## Arguments
- **x**
The number get the exponent portion.

## Library
libc

## Description
These functions return the exponent portion as a signed integer.

## Returns
- **the exponent of x**
If success.
&nbsp;

- **FP_ILOGBD**
If *x* is a zero.
&nbsp;

- **FP_ILOGBNAN**
If *x* is a NaN.
&nbsp;

- **INT_MAX**
If *x* is an infinity.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
