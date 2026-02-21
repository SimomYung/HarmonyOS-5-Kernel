# pthread\_cond\_init()
Initialize a condition variable

## Synopsis
```c
#include <pthread.h>

int pthread_cond_init(pthread_cond_t *cond,
                      const pthread_condattr_t *attr);
```

## Arguments

- **cond**
A pointer to the condition variable.
&nbsp;
- **attr**
A pointer to the condition variable attributes object.

## Library
libc

## Description
The pthread\_cond\_init() function initializes the condition variable specified by *cond* with an optional attributes object specified by *attr*. If you pass NULL as *attr*, then the default attributes will be used, in this case, assigning PTHREAD\_COND\_INITIALIZER to the condition variable directly can achieve the same effect.
&nbsp;
After initialization, the condition variable can be used to perform synchronization.

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
