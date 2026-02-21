# lgamma(), lgammaf(), lgammal(), lgamma_r(), lgammaf_r(), lgammal_r() signgam
Log gamma functions

## Synopsis

```c
#include <math.h>

double lgamma(double x);
float lgammaf(float x);
long double lgammal(long double x);
double lgamma_r(double x, int *signp);
float lgammaf_r(float x, int *signp);
long double lgammal_r(long double x, int *signp);
extern int signgam;
```

## Arguments

- **x**
  The Floating-point number that to be computed.
&nbsp;
- **signp**
  A pointer to a location where the function can store the sign of gamma function.

## Library
libc

## Description
These functions return the logarithm of the gamma function of *x*, that is
$log_{e}\left | \int_{0}^{\infty }t^{x-1}e^{-t}dt\right |$.
&nbsp;
The functions lgamma_r(), lgammaf_r(), and lgammal_r() are thread-safe, they return the sign by the argument *signp*.
&nbsp;
If *x* is NaN, negative infinity, or a negative integer, the value of signgam is unspecified.
&nbsp;
These functions will not set floating-point status flag, because the relevant instructions can not generate floating-point exception results. and these functions will not set errno currently.

## Returns

Upon successful completion, these functions return the logarithm of the gamma function of *x*.
&nbsp;
If *x* is NaN, NaN is returned.
&nbsp;
If *x* is 1 or 2, 0 is returned.
&nbsp;
If *x* is positive infinity or negative infinity, positive infinity is returned.
&nbsp;
If *x* is a non-positive integer, +HUGE_VAL, +HUGE_VALF, or +HUGE_VALL is returned, respectively.
&nbsp;
If the result overflows, return HUGE_VAL, HUGE_VALF, or HUGE_VALL, respectively.

## Errors

N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety

TODO