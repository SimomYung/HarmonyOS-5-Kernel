# srand48()
Set the seed for a new sequence of pseudo-random integers

## Synopsis
```c
#include <stdlib.h>

void srand48(long seed);
```

## Arguments 

- **seed**
The seed for a new sequence of pseudo-random integers.

## Library
libc

## Description

The srand48() function sets the seed of pseudo-random integers which is used for generating 48-bit pseudo-random integer. This function should be called before using rand48().

## Returns
N/A

## Errors
N/A

## Examples
&nbsp;

## Classifications
POSIX 1003.1-2017

## Function Safety
TODO
