# log1p(), log1pf(), log1pl()
Logarithm of 1 plus argument functions

## Synopsis
```c
#include <math.h>

double log1p(double x);
float log1pf(float x);
long double log1pl(long double x);
```

## Arguments
- **x**
The number to be calculated.

## Library
libc

## Description
These functions return log(1 + *x*).

## Returns

- **the natural logarithm of (1 + x)**
If success.
&nbsp;

- **NaN**
If *x* is a NaN.
If *x* is less than -1.
&nbsp;

- **positive infinity**
If *x* is a positive infinity.
&nbsp;

- **-HUGE_VAL/-HUGE_VALF/-HUGE_VALL**
If *x* is -1.


## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
