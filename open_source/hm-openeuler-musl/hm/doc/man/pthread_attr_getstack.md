# pthread\_attr\_getstack()
Get the stack attribute in the thread attributes object.

## Synopsis
```c
#include <pthread.h>

int pthread_attr_getstack(const pthread_attr_t *attr,
                          void **stackaddr, size_t *stacksize);
```

## Arguments

- **attr**
A pointer to the thread attributes object.
&nbsp;
- **stackaddr**
A pointer to the buffer which stores the returned stack address value.
&nbsp;
- **stacksize**
A pointer to the buffer which stores the returned stack size value.

## Library
libc

## Description
The pthread\_attr\_getstack() function obtains the stack address and stack size set in *attr* and stores them in the buffer specified in *stackaddr* and *stacksize* respectively.
&nbsp;
Note that you should call this function after calling pthread\_attr\_setstack() to set a valid stack attribute in *attr*, or you may get an error.
&nbsp;
Calling pthread\_attr\_getstack() if the *attr* is uninitialized results in undefined behavior.

## Returns

- **0**
Success.
&nbsp;
- **EINVAL**
The stack address stored in *attr* is 0, that is to say, you have not set a valid stack attribute through pthread\_attr\_setstack().

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
