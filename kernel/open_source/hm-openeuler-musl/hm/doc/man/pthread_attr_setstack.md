# pthread\_attr\_setstack()
Set the stack attribute in the thread attributes object.

## Synopsis
```c
#include <pthread.h>

int pthread_attr_setstack(pthread_attr_t *attr,
                          void *stackaddr, size_t stacksize);
```

## Arguments

- **attr**
A pointer to the thread attributes object.
&nbsp;
- **stackaddr**
The stack address to be set.
&nbsp;
- **stacksize**
The stack size to be set.

## Library
libc

## Description
The pthread\_attr\_setstack() function sets the stack address and size in the thread attributes object specified by *attr*. *stackaddr* should be the lowest address of a buffer allocated by the caller, size of this buffer is *stacksize*.
&nbsp;
New thread created with *attr* will use this buffer as stack, so remember to change *stackaddr* attribute if you want to create multiple threads with the same *attr*.
&nbsp;
Calling pthread\_attr\_setstack() if the *attr* is uninitialized results in undefined behavior.

## Returns

- **0**
Success.
&nbsp;
- **EINVAL**
The parameter stacksize is invalid (larger than ULONG\_MAX/4).

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
