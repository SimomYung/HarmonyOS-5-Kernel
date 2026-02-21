# srand()
Set the seed for a new sequence of pseudo-random integers

## Synopsis
```c
#include <stdlib.h>

void srand(unsigned s);
```

## Arguments 

- **s**
The seed for a new sequence of pseudo-random integers.

## Library
libc

## Description

The srand() function sets the seed of pseudo-random integers by the argument. If calling srand() with the same argument, the seed will be the same. The default value of the seed is 1.

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
