# copysign(), copysignf(), copysignl()
Copy sign of a number

## Synopsis

```c
#include <math.h>

double copysign(double x, double y);
float copysignf(float x, float y);
long double copysignl(long double x, long double y);
```

## Arguments

- **x**
  Provide the magnitude of the resulting value.
&nbsp;

- **y**
  Provide the sign of the resulting value.


## Library
libc

## Description
These functions return a value with the magnitude of *x* and the sign of *y*.

## Returns

Upon successful completion, these functions return a value with the magnitude of *x* and the sign of *y*.
&nbsp;
If *x* is a NaN, a NaN with the sign bit of *y* is returned.


## Errors

N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety

TODO