# pthread\_condattr\_init()
Initialize the condition variable attributes object

## Synopsis
```c
#include <pthread.h>

int pthread_condattr_init(pthread_condattr_t *attr);
```

## Arguments

- **attr**
A pointer to the condition variable attributes object.

## Library
libc

## Description
The pthread\_condattr\_init() function initializes the condition variable attributes object specified by *attr* with the default attributes. You can change multiple attributes with a series of pthread\_condattr\_set prefixed functions. Initialized attributes object can be used in one or more pthread\_cond\_init() calls to initialize condition variables.

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
