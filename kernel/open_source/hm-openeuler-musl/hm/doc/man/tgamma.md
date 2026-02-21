# tgamma(), tgammaf(), tgammal()
Gamma functions

## Synopsis
```c
#include <math.h>

double tgamma(double x);
float tgammaf(float x);
long double tgammal(long double x);
```

## Arguments

- **x**
The number to be calculated in Gamma functions.

## Library
libc

## Description
These functions calculate the Gamma functions with the argument *x*.
&nbsp;
Gamma function is defined as:
$$
\Gamma(x) = \int_{0}^{\infty}t^{x-1}e^{-t}dt
$$

## Returns

- **Gamma(x)**
If success.
&nbsp;

- **NaN**
If *x* is a NaN.
If *x* is a infinity.
&nbsp;

- **positive infinity**
If *x* is a positive infinity.
&nbsp;

- **HUGE_VAL/HUGE_VALF/HUGE_VALL**
If the result overflows, and the return value has the correct mathematical sign.
If *x* is +0/-0, and the return value has the same sign with *x*.
&nbsp;

- **0**
If the result underflows, the return value 0 has the correct mathematical sign.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
