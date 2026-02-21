# pthread\_thread\_init()
initialize a barrier object

## Synopsis
```c
#include <pthread.h>

int pthread_barrier_init(pthread_barrier_t *barrier,
                         pthread_barrierattr_t *attr,
                         unsigned int count);
```

## Arguments

- **barrier**
A pointer to the thread barrier object.
&nbsp;
- **attr**
Specify the attributes of barrier object
&nbsp;
- **count**
Specify the threshold

## Library
libc

## Description
The pthread_barrier_init() function inits a new barrier object where attr specifies the attributes and count specifies the threshold. If attr is NULL, the default attributes will be used. The count argument is later used by the pthread_barrier_wait() function to check if the required number of threads reached the barrier.

## Returns

- **0**
Success.
&nbsp;
- **EINVAL**
The value specified by barrier or attr is invalid.
&nbsp;
- **ENOMEM**
The process cannot allocate enough memory to create another barrier object.
&nbsp;
- **ENOTSUP**
The attributes specified by attr are not supported by the current implementation.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO