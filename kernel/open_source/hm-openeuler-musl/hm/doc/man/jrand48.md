# jrand48()
Generate a pseudo-random signed long integer in a thread-safe manner

## Synopsis
```c
#include <stdlib.h>

long jrand48(unsigned short xsubi[3])
```
## Arguments
- **xsubi**
An array that comprises the 48 bits of the initial value that you want to use.

## Library
libc

## Description
The jrand48() function uses a linear congruential algorithm and 48-bit integer arithmetic to generate a signed long integer uniformly distributed over the interval [-2^31, 2^31).
&nbsp;
It's a thread-safe version of mrand48().
&nbsp;
The xsubi array should contain the desired initial value; this makes jrand48() thread-safe, and lets you start a sequence of random numbers at any known value.

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
