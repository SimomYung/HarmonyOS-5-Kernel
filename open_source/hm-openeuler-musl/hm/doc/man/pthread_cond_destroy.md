# pthread\_cond\_destroy()
Destroy a condition variable

## Synopsis
```c
#include <pthread.h>

int pthread_cond_destroy(pthread_cond_t *cond);
```

## Arguments

- **cond**
A pointer to the condition variable.

## Library
libc

## Description
The pthread\_cond\_destroy() function destroys the condition variable indicated by *cond*. Attempting to destroy a process-shared condition variable will wake all waiters that are currently blocked on the condition variable.

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
