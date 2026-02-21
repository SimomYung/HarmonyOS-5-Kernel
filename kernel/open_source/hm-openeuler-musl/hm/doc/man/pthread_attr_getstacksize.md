# pthread\_attr\_getstacksize()
Get the stack size attribute in the thread attributes object.

## Synopsis
```c
#include <pthread.h>

int pthread_attr_getstacksize(const pthread_attr_t *attr, size_t *stacksize);
```

## Arguments

- **attr**
A pointer to the thread attributes object.
&nbsp;
- **stacksize**
A pointer to the buffer which stores the returned stack size value.

## Library
libc

## Description
The pthread\_attr\_getstacksize() function obtains the stack size attribute set in *attr* and stores the value in the buffer specified by *stacksize*.

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
