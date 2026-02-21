# sincos(), sincosf(), sincosl()

Calculate sin and cos simultaneously

## Synopsis

```c
#define _GNU_SOURCE
#include <math.h>

void sincos(double x, double *sin, double *cos);
void sincosf(float x, float *sin, float *cos);
void sincosl(long double x, long double *sin, long double *cos);
```

## Arguments 

- **x**
Independent variable *x*.
&nbsp;
- **sin**
The result of sin.
&nbsp;
- **cos**
The result of cos.

## Library

libc

## Description

Several applications need sine and cosine of the same angle *x*.  These functions compute both at the same time, and store the results in \**sin* and \**cos*.  Using this function can be more efficient than two separate calls to sin() and cos().
&nbsp;
If *x* is a NaN, a NaN is returned in \**sin* and \**cos*.
&nbsp;
If *x* is positive infinity or negative infinity, a domain error occurs, and a NaN is returned in \**sin* and \**cos*.

## Returns

N/A

## Errors

N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
