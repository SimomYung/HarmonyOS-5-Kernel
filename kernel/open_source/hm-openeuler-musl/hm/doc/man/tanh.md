# tanh(), tanhf(), tanhl()

Hyperbolic tangent function

## Synopsis

```c
#include <math.h>

double tanh(double x);
float tanhf(float x);
long double tanhl(long double x);
```

## Arguments 

- **x**
Independent variable *x*.

## Library

libc

## Description

These functions return the hyperbolic tangent of *x*, which is defined mathematically as:
```
tanh(x) = sinh(x) / cosh(x)
```

## Returns

- **(-1, 1)**
On success, these functions return the hyperbolic tangent of *x*.
&nbsp;
- **NaN**
If *x* is a NaN, a NaN is returned.
&nbsp;
- **-1, +1**
If *x* is positive infinity (negative infinity), +1 (-1) is returned.

## Errors

N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO