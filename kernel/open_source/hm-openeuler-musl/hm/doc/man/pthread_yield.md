# pthread\_yield()
Yield the CPU

## Synopsis
```c
#define _GNU_SOURCE
#include <pthread.h>

int pthread_yield(void);
```

## Arguments
N/A

## Library
libc

## Description
The pthread\_yield() function causes the calling thread to yield the CPU so that another thread will be scheduled to run. The calling thread will be placed at the end of the run queue for its static priority.

## Returns

- **0**
Success.

## Errors
N/A

## Example
&nbsp;

## Classification
Unix

## Function Safety
TODO

