# pthread\_mutexattr\_destroy()
Destroy the mutex attributes object

## Synopsis
```c
#include <pthread.h>

int pthread_mutexattr_destroy(pthread_mutexattr_t *attr);
```

## Arguments

- **attr**
A pointer to the mutex attributes object.

## Library
libc

## Description
The pthread\_mutexattr\_destroy() function destroys the mutex attributes object specified by *attr* which is initialized by pthread\_mutexattr\_init().

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
