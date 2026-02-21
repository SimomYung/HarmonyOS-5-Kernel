# asinh(), asinhf(), asinhl()

Inverse hyperbolic sine function

## Synopsis

```c
#include <math.h>

double asinh(double x);
float asinhf(float x);
long double asinhl(long double x);
```

## Arguments 

- **x**
Independent variable *x*.

## Library

libc

## Description

These functions calculate the inverse hyperbolic sine of *x*. That is the value whose hyperbolic sine is *x*.

## Returns

- **(-∞, +∞)**
On success, these functions return the inverse hyperbolic sine of *x*.
&nbsp;
- **INFINITY**
If *x* is positive infinity (negative infinity), positive infinity (negative infinity) is returned.
&nbsp;
- **NaN**
If *x* is a NaN, a NaN is returned.

## Errors

N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO