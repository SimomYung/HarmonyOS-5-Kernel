# cosh(), coshf(), coshl()

Hyperbolic cosine function

## Synopsis

```c
#include <unistd.h>
#include <math.h>

double cosh(double x);
float coshf(float x);
long double coshl(long double x);
```

## Arguments 

- **x**
Independent variable *x*.

## Library

libc

## Description

These functions return the hyperbolic cosine of *x*, which is defined mathematically as:
```
cosh(x) = (exp(x) + exp(-x)) / 2
```

## Returns

- **[1, +∞)**
On success, these functions return the hyperbolic cosine of *x*.
&nbsp;
- **NaN**
If *x* is a NaN, a NaN is returned.
&nbsp;
- **INFINITY**
If *x* is positive infinity or negative infinity, positive infinity is returned.
&nbsp;
- **+HUGE_VAL, +HUGE_VALF, or +HUGE_VALL**
If the result overflows, a range error occurs, and the functions return +HUGE_VAL, +HUGE_VALF, or +HUGE_VALL, respectively.

## Errors

N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
