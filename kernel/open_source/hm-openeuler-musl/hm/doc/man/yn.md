# yn(), ynf()
Bessel functions of the second kind


## Synopsis
```c
#include <math.h>

double yn(int n, double x);
float ynf(int n, float x);
```

## Arguments

- **n**
The order of Bessel functions.
&nbsp;
- **x**
The number to be calculated in Bessel functions.

## Library
libc

## Description
These functions return Bessel functions of *x* of the second kind of order *n*.

## Returns

- **Bessel value of the second kind for *x* of order n**
If success.
&nbsp;

- **NaN**
If *x* is a NaN.
&nbsp;

- **-HUGE_VAL/-HUGE_VALF/-HUGE_VALL**
If the result overflows.
If *x* is negative.
If *x* is 0.0.
&nbsp;

- **0.0**
If the result underflows.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
