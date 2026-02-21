# pthread\_attr\_init()
Initialize a thread attributes object.

## Synopsis
```c
#include <pthread.h>

int pthread_attr_init(pthread_attr_t *attr);
```

## Arguments

- **attr**
A pointer to the thread attributes object.

## Library
libc

## Description
The pthread\_attr\_init() function initializes a thread attributes object with default attributes. You can change multiple attributes with a series of pthread\_attr\_set prefixed functions. Initialized attributes object can be used in one or more pthread\_create() calls to create threads.

## Returns

- **0**
These functions always succeed and return 0.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
