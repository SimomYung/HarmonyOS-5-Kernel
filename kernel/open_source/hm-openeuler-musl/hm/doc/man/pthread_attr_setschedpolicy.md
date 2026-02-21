# pthread\_attr\_setschedpolicy()
Set a thread's scheduling policy attributes

## Synopsis
```c
#include <pthread.h>

int pthread_attr_setschedpolicy(pthread_attr_t *attr, int policy);
```

## Arguments

- **attr**
A thread attributes object.
&nbsp;
- **policy**
The scheduling policy identifier.

## Library
libc

## Description
Set a thread's scheduling policy attributes from a thread attributes object *attr*. The scheduling policy is identified by *policy*. Currently policy can be set to SCHED_FIFO, SCHED_RR or SCHED_NORMAL.

RT policy can be set to SCHED_FIFO or SCHED_RR.

## Returns

- **0**
Success.
&nbsp;

- **EINVAL**
*param* is invalid for the thread's scheduling policy.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
