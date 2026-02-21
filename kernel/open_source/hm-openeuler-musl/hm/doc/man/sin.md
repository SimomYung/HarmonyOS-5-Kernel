# sin(), sinf(), sinl()

Sine function

## Synopsis

```c
#include <math.h>

double sin(double x);
float sinf(float x);
long double sinl(long double x);
```

## Arguments 

- **x**
Independent variable *x*.

## Library

libc

## Description

These functions return the sine of *x*, where *x* is given in radians.

## Returns

- **[-1, 1]**
On success, these functions return the sine of *x*.
&nbsp;
- **NaN**
If *x* is a NaN, a NaN is returned.
If *x* is positive infinity or negative infinity, a domain error occurs, and a NaN is returned.

## Errors

N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
