# jn(), jnf()
Bessel functions

## Synopsis
```c
#include <math.h>

double jn(int n, double x);
float jnf(int n, float x);

```

## Arguments
- **n**
Order number.
&nbsp;

- **x**
The number to be calculated in Bessel functions.

## Library
libc

## Description
The jn and jnf functions return Bessel functions of *x* of the first kind of order *n*.

## Returns
- **Bessel value of the first kind of order *n* for x**
If success.
&nbsp;

- **0**
If result underflows or *x* is too large.
&nbsp;

- **NaN**
If *x* is a NaN.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
