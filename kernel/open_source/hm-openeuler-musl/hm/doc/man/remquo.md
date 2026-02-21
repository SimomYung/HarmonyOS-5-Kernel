# remquo(), remquof(), remquol()
Functions to get remainder and part of quotient

## Synopsis
```c
#include <math.h>

double remquo(double x, double y, int *quo);
float remquof(float x, float y, int *quo);
long double remquol(long double x, long double y, int *quo);
```

## Arguments

- **x**
The number to be divided.
&nbsp;

- **y**
The number to divide.
&nbsp;

- **quo**
The pointer to restore part of the quotient.

## Library
libc

## Description
These functions calculate the remainder(the same as function remainder() do) which is returned as the result and part of the quotient of *x* over *y* (part of it is restored in pointer *quo*).
&nbsp;
The value restored in pointer *quo* has the same sign as *x / *y* and at least has 3 bits the same to the quotient.

## Returns

- **NaN**
If *x* or *y* is a NaN.
If *x* is an infinity and *y* is not a NaN.
If *x* is not a NaN and *y* = 0.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
