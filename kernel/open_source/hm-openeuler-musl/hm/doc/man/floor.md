# floor(), floorf(), floorl()
Round down a value to the next integer

## Synopsis

```c
#include <math.h>

double floor(double x);
float floorf(float x);
long double floorl(long double x);
```

## Arguments

- **x**
  Value to round down.


## Library
libc

## Description
These functions compute the largest integral value not greater than *x*.

## Returns

Upon successful completion, these functions return the largest integral value that is not greater than *x*.
&nbsp;
If *x* is NaN, a NaN is returned.
&nbsp;
If *x* is +0 or -0, *x* is returned.
&nbsp;
If *x* is positive infinity or negative infinity, *x* is returned.


## Errors

N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety

TODO