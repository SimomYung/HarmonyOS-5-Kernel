# pthread\_attr\_setstacksize()
Set the stack size attribute in the thread attributes object.

## Synopsis
```c
#include <pthread.h>

int pthread_attr_setstacksize(pthread_attr_t *attr, size_t stacksize);
```

## Arguments

- **attr**
A pointer to the thread attributes object.
&nbsp;
- **stacksize**
The stack size to be set.

## Library
libc

## Description
The pthread\_attr\_setstacksize() function sets the stack size in the thread attributes object specified by *attr*.
&nbsp;
If you pass NULL as attr parameter when calling pthread\_create(), then default stack size is 131072 bytes. Otherwise, at least *stacksize* bytes will be allocated to be used as stack for new threads created with *attr*.
&nbsp;
It is easier to trigger stack overflow with a small thread stack size (such as PTHREAD\_STACK\_MIN), so users should be cautious about setting the pthread stack size.

## Returns

- **0**
Success.
&nbsp;
- **EINVAL**
The parameter stacksize is invalid (less than PTHREAD\_STACK\_MIN (2048) bytes).

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
