# lround(), lroundf(), lroundl(), llround(), llroundf(), llroundl()
Round to nearest integer functions

## Synopsis

```c
#include <math.h>

long int lround(double x);
long int lroundf(float x);
long int lroundl(long double x);
long long int llround(double x);
long long int llroundf(float x);
long long int llroundl(long double x);
```

## Arguments

- **x**
  The Floating-point number that to be computed.

## Library
libc

## Description
These functions compute the nearest integer value to *x*, rounding halfway cases away from zero, regardless of the current rounding mode. These functions will not set errno currently.

## Returns

Upon successful completion, these functions return the rounded integer value.
&nbsp;
If *x* is NaN, the return value is unspecified.
&nbsp;
If *x* is positive infinity or negative infinity, the return value is unspecified.
&nbsp;
If the correct value is positive and too large to represent as a long (long long in the case of the ll* functions), the return value is unspecified.

## Errors

N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety

TODO
