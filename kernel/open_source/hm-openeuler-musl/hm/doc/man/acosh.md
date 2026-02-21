# acosh(), acoshf(), acoshl()

Inverse hyperbolic cosine function

## Synopsis

```c
#include <math.h>

double acosh(double x);
float acoshf(float x);
long double acoshl(long double x);
```

## Arguments 

- **x**
Independent variable *x*.

## Library

libc

## Description

These functions calculate the inverse hyperbolic cosine of *x*; that is the value whose hyperbolic cosine is *x*.

## Returns

- **[0, +∞)**
On success, these functions return the inverse hyperbolic cosine of *x*.
&nbsp;
- **INFINITY**
If *x* is positive infinity, positive infinity is returned.
&nbsp;
- **NaN**
If *x* is a NaN, a NaN is returned.
If *x* is less than 1, a domain error occurs, and the functions return a NaN.

## Errors

N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
