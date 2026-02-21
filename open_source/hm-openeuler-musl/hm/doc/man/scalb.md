# scalb(), scalbf()
Multiply floating-point number by integral power of radix

## Synopsis
```c
#include <math.h>

double scalb(double x, double exp);
float scalbf(float x, float exp);
```

## Arguments

- **x**
A multiplier.
&nbsp;

- **exp**
The exponent number.

## Library
libc

## Description
These functions return *x* * (FLT\_RADIX ^ *exp*).

## Returns

- **x * (FLT_RADIX ^ exp)**
If success.
&nbsp;

- **NaN**
If *x* or *exp* is a NaN.
If *x* = 0 and *exp* is not a positive infinity.
If *x* is an infinity and *exp* is a negative infinity.
&nbsp;

- **+0/-0**
If *x* is +0/-0 and *exp* is not an positive infinity.
If the result underflows, 0 is returned with the same sign as *x*.
&nbsp;

- **HUGE_VAL/HUGE_VALF/HUGE_VALL**
If the result overflows.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
