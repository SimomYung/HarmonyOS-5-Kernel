#  ccosh(), ccoshf(), ccoshl()
Compute the hyperbolic cosine value of the given complex number

## Synopsis
```c
#include <complex.h>

double complex ccosh(double complex c);
float complex ccoshf(float complex c);
long double complex ccoshl(long double complex c);
```

## Arguments
- **c**
A complex number.

## Library
libc

## Description
These functions compute the hyperbolic cosine value of the given complex number *c*. The result is equal to:
```
(exp(c)+exp(-c))/2
```

## Returns
The hyperbolic cosine value of the given complex number.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
