# lrand48()
Generate a pseudo-random nonnegative long integer

## Synopsis
```c
#include <stdlib.h>

long lrand48(void)
```
## Arguments
N/A

## Library
libc

## Description
The lrand48() function uses a linear congruential algorithm and 48-bit integer arithmetic to generate a nonnegative long integer uniformly distributed over the interval [0, 2^31].

## Returns
A pseudo-random long integer.

## Errors
N/A

## Examples
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
