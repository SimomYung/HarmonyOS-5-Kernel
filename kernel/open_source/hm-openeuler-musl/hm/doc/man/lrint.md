# lrint(), lrintf(), lrintl(), llrint(), llrintf(), llrintl()
Round to nearest integer

## Synopsis
```c
#include <math.h>

long int lrint(double x);
long int lrintf(float x);
long int lrintl(long double x);

long long int llrint(double x);
long long int llrintf(float x);
long long int llrintl(long double x);

```

## Arguments
- **x**
The number to be rounded.

## Library
libc

## Description
These functions round *x* to the nearest integer value, the round direction can be changed using function fesetround().

## Returns
- **the rounded integer value**
If success.
&nbsp;

- **unspecified value**
If *x* is a NaN or an infinity or the value of *x* is too large to be stored in *x*.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
