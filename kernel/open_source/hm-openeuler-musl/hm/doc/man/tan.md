# tan(), tanf(), tanl()

Tangent function

## Synopsis

```c
#include <math.h>

double tan(double x);
float tanf(float x);
long double tanl(long double x);
```

## Arguments 

- **x**
Independent variable *x*.

## Library

libc

## Description

These functions return the tangent of *x*, where *x* is given in radians.

## Returns

- **(-∞, +∞)**
On success, these functions return the tangent of *x*.
&nbsp;
- **NaN**
If *x* is a NaN, a NaN is returned.
If *x* is positive infinity or negative infinity, a domain error occurs, and a NaN is returned.
&nbsp;
- **HUGE_VAL, HUGE_VALF, or HUGE_VALL**
If the correct result would overflow, a range error occurs, and the functions return HUGE_VAL, HUGE_VALF, or HUGE_VALL, respectively, with the mathematically correct sign.

## Errors

N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
