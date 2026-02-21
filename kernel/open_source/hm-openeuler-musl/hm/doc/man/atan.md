# atan(), atanf(), atanl()

Arc tangent function

## Synopsis

```c
#include <math.h>

double atan(double x);
float atanf(float x);
long double atanl(long double x);
```

## Arguments

- **x**
Independent variable *x*.

## Library

libc

## Description

These functions calculate the principal value of the arc tangent of *x*. That is the value whose tangent is *x*.

## Returns

- **[-pi/2, pi/2]**
On success, these functions return the principal value of the arc tangent of *x* in radians; the return value is in the range [-pi/2, pi/2].
If *x* is positive infinity (negative infinity), +pi/2 (-pi/2) is returned.
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
