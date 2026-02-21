# pthread\_mutexattr\_init()
Initialize the mutex attributes object

## Synopsis
```c
#include <pthread.h>

int pthread_mutexattr_init(pthread_mutexattr_t *attr);
```

## Arguments

- **attr**
A pointer to the mutex attributes object.

## Library
libc

## Description
The pthread\_mutexattr\_init() function initializes a mutex attributes object with the default attributes. You can change multiple attributes with a series of pthread\_mutexattr\_set prefixed functions. Initialized attributes object can be used in one or more pthread\_mutex\_init() calls to initialize mutexes.
&nbsp;
Initializing an already initialized attributes results in undefined behavior.

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
