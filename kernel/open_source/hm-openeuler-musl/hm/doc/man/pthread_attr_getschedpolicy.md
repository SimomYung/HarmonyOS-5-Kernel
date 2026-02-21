# pthread\_attr\_getschedpolicy()
Get a thread's scheduling policy attributes

## Synopsis
```c
#include <pthread.h>

int pthread_attr_getschedpolicy(const pthread_attr_t *attr, int *policy);
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
Get a thread's scheduling policy attributes in a thread attributes object *attr*. The scheduling policy is identified by *policy*.

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
