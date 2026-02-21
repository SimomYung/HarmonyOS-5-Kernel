# pthread\_mutexattr\_gettype()
Get the mutex type attribute

## Synopsis
```c
#include <pthread.h>

int pthread_mutexattr_gettype(const pthread_mutexattr_t *attr, int *type);
```

## Arguments

- **attr**
A pointer to the mutex attributes object.
&nbsp;
- **type**
A pointer to the buffer which stores the returned mutex type value.

## Library
libc

## Description
The pthread\_mutexattr\_gettype() function obtains the mutex type value set in *attr* and stores it in the buffer indicated by *type*.

## Returns

- **0**
This function always succeeds.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
