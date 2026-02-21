# pthread\_attr\_getdetachstate()
Get the detach state attribute in the thread attributes object.

## Synopsis
```c
#include <pthread.h>

int pthread_attr_getdetachstate(const pthread_attr_t *attr, int *detachstate);
```

## Arguments

- **attr**
A pointer to the thread attributes object.
&nbsp;
- **detachstate**
A pointer to the buffer which stores the returned detach state value.

## Library
libc

## Description
The pthread\_attr\_getdetachstate() function obtains the detach state value set in *attr* and stores it in the buffer indicated by *detachstate*.
&nbsp;
Calling pthread\_attr\_getdetachstate() if the *attr* is uninitialized results in undefined behavior.

## Returns

- **0**
This function always succeeds and returns 0.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
