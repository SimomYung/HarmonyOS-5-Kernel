# pthread\_attr\_setguardsize()
Set the guard size attribute in the thread attributes object.

## Synopsis
```c
#include <pthread.h>

int pthread_attr_setguardsize(pthread_attr_t *attr, size_t guardsize);
```

## Arguments

- **attr**
A pointer to the thread attributes object.
&nbsp;
- **guardsize**
Guard size to be set in *attr*.

## Library
libc

## Description
The pthread\_attr\_setguardsize() function sets the value specified by *guardsize* to the thread attributes object specified by *attr*.
&nbsp;
If you pass NULL as attr parameter when calling pthread\_create(), then default guard size is 8192 bytes. Otherwise, for each new thread created with *attr*, the system allocates an extra memory at the overflow end of the stack, actual size of this memory is the value of *guardsize* rounded up to a multiple of PAGE\_SIZE. Extra guard memory cannot be read or written, so any stack overflow may cause coredump.
&nbsp;
If *guardsize* is 0, or the stack address attribute is set in *attr* by pthread\_attr\_setstacksize() or pthread\_attr\_setstack(), then guardsize is ignored, and new threads created with *attr* don't have a guard area at end of the stack. It's the responsibility of the caller to manage possible stack overflow.
&nbsp;
Calling pthread\_attr\_setguardsize() if the *attr* is uninitialized results in undefined behavior.

## Returns

- **0**
Success.
&nbsp;
- **EINVAL**
The parameter guardsize is invalid (larger than ULONG\_MAX/8).

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
