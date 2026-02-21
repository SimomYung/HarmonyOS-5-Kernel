# pthread\_condattr\_destroy()
Destroy the condition variable attributes object

## Synopsis
```c
#include <pthread.h>

int pthread_condattr_destroy(pthread_condattr_t *attr);
```

## Arguments

- **attr**
A pointer to the condition variable attributes object.

## Library
libc

## Description
The pthread\_condattr\_destroy() function destroys the condition variable attributes object specified by *attr* that is initialized through pthread\_condattr\_init() function.

## Returns

- **0**
This function always succeeds and returns 0.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
