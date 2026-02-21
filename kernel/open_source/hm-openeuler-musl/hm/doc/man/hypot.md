# hypot(), hypotf(), hypotl()
Euclidean distance functions

## Synopsis
```
#include<math.h>

double hypot(double x, double y);
float hypotf(float x, float y);
long double hypotl(long double x, long double y);
```

## Arguments
- **x**
A side of a right triangle.
&nbsp;
- **y**
The other side of the right triangle.

## Library
libc

## Description
Given two sides of a right triangle, these functions will calculate the length of the hypotenuse. In other words, these functions return sqrt(x\*x, y\*y);

## Returns
- **Length of the hypotenuse**
On success, these functions return the length of the hypotenuse of a right triangle whose right-angle sides are *x* and *y*.
&nbsp;

- **positive infinity**
While *x* or *y* is an infinity.
&nbsp;

- **HUGE\_VAL/HUGE\_VALF/HUGE\_VALL**
If the result overflows, these functions will return HUGE\_VAL, HUGE\_VALF, HUGE\_VALL.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
