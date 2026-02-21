# pthread\_condattr\_getclock()
Get the clock selection condition variable attribute

## Synopsis
```c
#include <pthread.h>

int pthread_condattr_getclock(const pthread_condattr_t *attr,
                              clockid_t *clock_id);
```

## Arguments

- **attr**
A pointer to the condition variable attributes object.
&nbsp;
- **clock_id**
A pointer to the buffer which stores the return clock ID.

## Library
libc

## Description
The pthread\_condattr\_getclock() function obtains the clock ID set in *attr* and places the value in the buffer pointed to by *clock_id*.

## Returns

- **0**
This function always succeeds.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
