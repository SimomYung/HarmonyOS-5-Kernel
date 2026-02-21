# pthread\_thread\_destory()
destroy a barrier object

## Synopsis
```c
#include <pthread.h>

int pthread_barrier_destroy(pthread_barrier_t *barrier);
```

## Arguments

- **barrier**
A pointer to the thread barrier object.

## Library
libc

## Description
The pthread_barrier_destroy() function frees the resources allocated for barrier.

## Returns

- **0**
Success.
&nbsp;
- **EINVAL**
The value specified by barrier is invalid.
&nbsp;
- **EBUSY**
There are still threads waiting on the barrier.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO