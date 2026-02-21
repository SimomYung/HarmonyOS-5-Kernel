# pthread\_attr\_getschedparam()
Get a thread's parameters of scheduling from a thread attributes object

## Synopsis
```c
#include <pthread.h>

int pthread_attr_getschedparam(const pthread_attr_t *attr, struct sched_param *param);

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
Get a thread's parameters of scheduling from a thread attributes object *attr*. The results are filled in *param*.

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
