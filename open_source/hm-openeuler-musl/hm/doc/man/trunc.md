# trunc(), truncf(), truncl()
Round a number to the nearest integer, of which the absolute value is smaller

## Synopsis
```c
#include <math.h>

double trunc(double x);
float truncf(float x);
long double truncl(long double x);

Link with -lm.
```

## Arguments

- **x**
The variable.

## Library
libc

## Description
Round *x* to its nearest integer, of which the absolute value is smaller.

## Returns
The rounded integer. If *x* is integral, infinite, or NaN, return *x* itself.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
