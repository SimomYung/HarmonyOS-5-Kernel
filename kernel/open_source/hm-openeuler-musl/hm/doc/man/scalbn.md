# scalbn(), scalbnf(), scalbnl(), scalbln(), scalblnf(), scalblnl()
Multiply floating-point number by integral power of radix

## Synopsis
```c
#include <math.h>

double scalbln(double x, long int exp);
float scalblnf(float x, long int exp);
long double scalblnl(long double x, long int exp);

double scalbn(double x, int exp);
float scalbnf(float x, int exp);
long double scalbnl(long double x, int exp);.
```

## Arguments

- **x**
A multiplier.
&nbsp;

- **exp**
The exponent.

## Library
libc

## Description
These functions return *x* * (FLT\_RADIX ^ *exp*), FLT\_RADIX is defined in &lt;float.h&gt;.

## Returns

- **NaN**
If *x* is a NaN.
&nbsp;

- **positive/negative infinity**
If *x* is positive/negative infinity.
&nbsp;

- **+0/-0**
If *x* is +0/-0;
If the results underflows, these functions return 0 with the same sign as *x*.
&nbsp;

- **HUGE_VAL/HUGE_VALF/HUGE_VALLL**
If the results overflows, the return value has the same sign as *x*.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
