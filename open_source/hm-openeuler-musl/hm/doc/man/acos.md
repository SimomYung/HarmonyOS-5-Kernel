# acos(), acosf(), acosl()

Arc cosine function

## Synopsis

```c
#include <math.h>

double acos(double x);
float acosf(float x);
long double acosl(long double x);
```

## Arguments 

- **x**
Independent variable *x*.

## Library

libc

## Description

These functions calculate the arc cosine of *x*; that is the value whose cosine is *x*.

## Returns

- **The range [0, pi]**
On success, these functions return the arc cosine of *x* in radians. The return value is in the range [0, pi].
&nbsp;
- **+0**
If *x* is +1, +0 is returned.
&nbsp;
- **NaN**
If *x* is a NaN, a NaN is returned.
If *x* is positive infinity or negative infinity, a domain error occurs, and a NaN is returned.
If *x* is outside the range [-1, 1], a domain error occurs, and a NaN is returned.

## Errors

N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
