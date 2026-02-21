# srandom(), srandom\_r()
Set the seed for a new sequence of pseudo-random integers

## Synopsis
```c
#include <stdlib.h>

void srandom(unsigned seed);
int srandom_r(unsigned seed, struct random_data *data);
```

## Arguments

- **seed**
The seed for a new sequence of pseudo-random integers.
&nbsp;
- **data**
The pointer contains the state information for generating the pseudo-random num.

## Library
libc

## Description
The srandom() function sets the seed of pseudo-random integers by the argument. If calling srandom() with the same argument, the seed will be same. The default value of the seed is 1.

The srandom\_r() function is the reentrant version of the srandom() function, it updates the state information which stored in the argument *data by the argument *seed*.

## Returns
The srandom\_r() function shall return as follow:

- **0**
The function sets the seed successfully.
&nbsp;
- **-1**
An error occurred.

## Errors
- **EINTR**
The argument *data* is invalid pointer.

## Examples
&nbsp;

## Classifications
POSIX 1003.1-2017

## Function Safety
TODO
