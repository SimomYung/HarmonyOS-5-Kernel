# setstate()
Reset the state of a pseudo-random number generator

## Synopsis
```c
#include <stdlib.h>;

char *setstate(char *state);
```
## Arguments
- **state**
A pointer to the state array that you want to use.

## Library
libc

## Description
Once the state of the pseudo-random number generator has been initialized, setstate() allows switching between state arrays.
&nbsp;
The array defined by the state argument is used for further random-number generation until initstate() is called or setstate() is called again.
&nbsp;
The setstate() function returns a pointer to the previous state array.

## Returns
On success, setstate() returns a pointer to the previous state array.
&nbsp;
On error, it returns NULL, with errno set to indicate the cause of the error.

## Errors
- **EINVAL**
A pointer to the previous state array, or NULL if an error occurred.

## Examples
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
