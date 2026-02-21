# pthread\_attr\_setschedparam()
Set a thread's scheduling policy and its parameters from a thread attributes object

## Synopsis
```c
#include <pthread.h>

int pthread_attr_setschedparam(pthread_attr_t *attr, const struct sched_param *param);

struct sched_param {
    int sched_priority;
};
```

## Arguments

- **attr**
A thread attributes object.
&nbsp;
- **param**
A structure of scheduling policy parameters in the form of struct sched_param.


## Library
libc

## Description
Set a thread's parameters of scheduling in a thread attributes object *attr* from *param*. For RT policy: SCHED_FIFO, SCHED_RR, the permitted range for sched_priority in *param* is [1, 99].

The sched_priority of SCHED_NORMAL is always 0.

## Returns

- **0**
Success.

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
