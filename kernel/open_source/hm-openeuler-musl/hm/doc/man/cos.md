# cos(), cosf(), cosl()

Cosine function

## Synopsis

```c
#include <math.h>

double cos(double x);
float cosf(float x);
long double cosl(long double x);
```

## Arguments 

- **x**
Independent variable *x*.

## Library

libc

## Description

These functions return the cosine of *x*, where *x* is given in radians.

## Returns

- **[-1, 1]**
On success, these functions return the cosine of *x*.
&nbsp;
- **NaN**
If *x* is a NaN, a NaN is returned.
If *x* is positive infinity or negative infinity, a domain error occurs, and a NaN is returned.

## Errors

N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
