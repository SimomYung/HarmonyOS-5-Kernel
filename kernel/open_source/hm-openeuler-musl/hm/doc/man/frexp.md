# frexp(), frexpf(), frexpl()
Floating-point evaluate functions

## Synopsis
```
#include<math.h>

double frexp(double x, int *exp);
float frexpf(float x, int *exp);
long double frexpl(long double x, int *exp);
```
## Arguments
- **x**
The Floating-point number that to be evaluated.
&nbsp;
- **exp**
A pointer to an object that stores the value of the index evaluated from *x*.

## Library
libc

## Description
These functions are used to split the Float-point number *x* into mantissa and exponent which is stored in *exp*.

## Returns
This function returns a normalized decimal. If the parameter *x* is not zero, the normalized decimal is *x* times a power of 2, and its absolute values range from 1/2 (inclusive) to 1 (exclusive). If *x* is zero, the normalized decimal is zero, and the zero is stored in *exp*.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## unction Safety
TODO
