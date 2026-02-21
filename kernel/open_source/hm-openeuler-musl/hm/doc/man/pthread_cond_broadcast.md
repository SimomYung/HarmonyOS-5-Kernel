# pthread\_cond\_broadcast()
Broadcast a condition variable

## Synopsis
```c
#include <pthread.h>

int pthread_cond_broadcast(pthread_cond_t *cond);
```

## Arguments

- **cond**
A pointer to the condition variable.

## Library
libc

## Description
The pthread\_cond\_broadcast() function wakes all threads currently blocked on the condition variable specified by *cond*. If more than one thread is blocked on the condition variable, the order of which they are awakened is undefined.
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
