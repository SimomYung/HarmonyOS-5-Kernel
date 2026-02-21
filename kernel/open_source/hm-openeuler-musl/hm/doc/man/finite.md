# finite(), finitef()
Floating-point classification functions

## Synopsis
```c
#include <math.h>

int finite(double x);
int finitef(float x);
```

## Arguments

- **x**
  The Floating-point number to be computed.

## Library
libc

## Description
These functions return a nonzero value if *x* is neither infinite nor NaN, In other cases, return 0.

## Returns

- **non-zero**
  *x* is infinite or NaN.
&nbsp;
- **0**
  *x* is not infinite or NaN.

## Errors

N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety

TODO