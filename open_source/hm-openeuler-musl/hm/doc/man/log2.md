# log2(), log2f(), log2l()
Base-2 logarithmic functions

## Synopsis
```c
#include <math.h>

double log2(double x);
float log2f(float x);
long double log2l(long double x);
```

## Arguments
- **x**
The number to be calculated.

## Library
libc

## Description
These functions return the base 2 logarithm of *x*.

## Returns

- **the base 2 logarithm of x**
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
