# atanh(), atanhf(), atanhl()

Inverse hyperbolic tangent function

## Synopsis

```c
#include <math.h>

double atanh(double x);
float atanhf(float x);
long double atanhl(long double x);
```

## Arguments 

- **x**
Independent variable *x*.

## Library

libc

## Description

These functions calculate the inverse hyperbolic tangent of *x*; that is the value whose hyperbolic tangent is *x*.

## Returns

- **(-∞, +∞)**
On success, these functions return the inverse hyperbolic tangent of *x*.
&nbsp;
- **NaN**
If *x* is a NaN, a NaN is returned.
If the absolute value of *x* is greater than 1, a domain error occurs, and a NaN is returned.
&nbsp;
- **HUGE_VAL, HUGE_VALF, or HUGE_VALL**
If *x* is +1 or -1, a pole error occurs, and the functions return HUGE_VAL, HUGE_VALF, or HUGE_VALL, respectively, with the mathematically correct sign.

## Errors

N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
