# ccos(), ccosf(), ccosl()
Compute the cosine value of the given complex number

## Synopsis
```c
#include <complex.h>

double complex ccos(double complex c);
float complex ccosf(float complex c);
long double complex ccosl(long double complex c);
```

## Arguments
- **c**
A complex number.

## Library
libc

## Description
These functions compute the cosine value of the given complex number *c*. The result is equal to:
```
(exp(i * c) + exp(-i * c)) / 2
```

## Returns
The cosine value of the given complex number.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
