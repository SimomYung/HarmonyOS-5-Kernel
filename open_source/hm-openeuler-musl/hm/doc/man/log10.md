# log10(), log10f(), log10l()
Base-10 logarithmic functions

## Synopsis
```c
#include <math.h>

double log10(double x);
float log10f(float x);
long double log10l(long double x);
```

## Arguments
- **x**
The number to be calculated.

## Library
libc

## Description
These functions return the base 10 logarithm of *x*.

## Returns

- **the base 10 logarithm of x**
If success.
&nbsp;

- **+0**
If *x* is 1.
&nbsp;

- **NaN**
If *x* is a NaN.
If *x* is negative.
&nbsp;

- **positive infinity**
If *x* is a positive infinity.
&nbsp;

- **-HUGE_VAL/-HUGE_VALF/-HUGE_VALL**
If *x* is 0.


## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
