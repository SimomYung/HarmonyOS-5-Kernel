# sinh(), sinhf(), sinhl()

Hyperbolic sine function

## Synopsis

```c
#include <math.h>

double sinh(double x);
float sinhf(float x);
long double sinhl(long double x);
```

## Arguments 

- **x**
Independent variable *x*.

## Library

libc

## Description

These functions return the hyperbolic sine of *x*, which is defined mathematically as:
```
sinh(x) = (exp(x) - exp(-x)) / 2
```

## Returns

- **(-∞, +∞)**
On success, these functions return the hyperbolic sine of *x*.
&nbsp;
- **NaN**
If *x* is a NaN, a NaN is returned.
&nbsp;
- **INFINITY**
If *x* is positive infinity (negative infinity), positive infinity (negative infinity) is returned.
&nbsp;
- **HUGE_VAL, HUGE_VALF, or HUGE_VALL**
If the result overflows, a range error occurs, and the functions return HUGE_VAL, HUGE_VALF, or HUGE_VALL, respectively, with the same sign as *x*.

## Errors

N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
