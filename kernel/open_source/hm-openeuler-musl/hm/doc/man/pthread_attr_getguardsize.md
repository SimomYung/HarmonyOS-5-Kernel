# pthread\_attr\_getguardsize()
Get the guard size attribute in the thread attributes object.

## Synopsis
```c
#include <pthread.h>

int pthread_attr_getguardsize(const pthread_attr_t *attr, size_t *guardsize);
```

## Arguments

- **attr**
A pointer to the thread attributes object.
&nbsp;
- **guardsize**
A pointer to the buffer which stores the returned guardsize value.

## Library
libc

## Description
The pthread\_attr\_getguardsize() function obtains the guardsize value set in *attr* and stores it in the buffer indicated by *guardsize*.
&nbsp;
Calling pthread\_attr\_getguardsize() if the *attr* is uninitialized results in undefined behavior.

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
