# pthread\_cond\_signal()
Signal a condition variable

## Synopsis
```c
#include <pthread.h>

int pthread_cond_signal(pthread_cond_t *cond);
```

## Arguments

- **cond**
A pointer to the condition variable.

## Library
libc

## Description
The pthread\_cond\_signal() function wakes a random thread (if more than one thread is blocked on the condition variable) currently blocked on the condition variable specified by *cond*.
&nbsp;
If there is no thread blocking on the condition variable, this function will do nothing.

## Returns

- **0**
Success.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
