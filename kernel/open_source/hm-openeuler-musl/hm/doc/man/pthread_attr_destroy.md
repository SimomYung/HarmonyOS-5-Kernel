# pthread\_attr\_destroy()
Destroy a thread attributes object.

## Synopsis
```c
#include <pthread.h>

int pthread_attr_destroy(pthread_attr_t *attr);
```

## Arguments

- **attr**
A pointer to the thread attributes object.

## Library
libc

## Description
Destroy a thread attributes object which is initialized by pthread\_attr\_init().
&nbsp;
Referencing a destroyed attributes object results in undefined behavior, unless it is reinitialized using pthread\_mutexattr\_init().

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
