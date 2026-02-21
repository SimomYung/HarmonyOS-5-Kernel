# pthread\_thread\_wait()
synchronize at a barrier

## Synopsis
```c
#include <pthread.h>

int pthread_barrier_wait(pthread_barrier_t *barrier);
```

## Arguments

- **barrier**
A pointer to the thread barrier object.

## Library
libc

## Description
The pthread_barrier_wait() function blocks the calling thread until the required number of threads call pthread_barrier_wait(), as specified at the object's initialization.

## Returns

- **0**
Success for each of the other threads.
&nbsp;
- **PTHREAD_BARRIER_SERIAL_THREAD**
Success for a single arbitrary thread.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO