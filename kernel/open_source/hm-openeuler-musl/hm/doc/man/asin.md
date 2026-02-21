# asin(), asinf(), asinl()

Arc sine function

## Synopsis

```c
#include <math.h>

double asin(double x);
float asinf(float x);
long double asinl(long double x);
```

## Arguments 

- **x**
Independent variable *x*.

## Library

libc

## Description

These functions calculate the principal value of the arc sine of *x*. That is the value whose sine is *x*.

## Returns

- **[-pi/2, pi/2]**
On success, these functions return the principal value of the arc sine of *x* in radians; the return value is in the range [-pi/2, pi/2].
&nbsp;
- **NaN**
If *x* is outside the range [-1, 1], a domain error occurs, and a NaN is returned.

## Errors

N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
