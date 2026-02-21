# pthread\_mutexattr\_setpshared()
Set the process-shared mutex attribute

## Synopsis
```c
#include <pthread.h>

int pthread_mutexattr_setpshared(pthread_mutexattr_t *attr,
                                 int pshared);
```

## Arguments

- **attr**
A pointer to the mutex attributes object.
&nbsp;
- **pshared**
The process-shared mutex attribute value to be set.

## Library
libc

## Description
The pthread\_mutexattr\_setpshared() function sets the process-shared mutex attribute specified by *pshared* in *attr*. Valid values are PTHREAD\_PROCESS\_PRIVATE and PTHREAD\_PROCESS\_SHARED.
&nbsp;
PTHREAD\_PROCESS\_PRIVATE is the default value in *attr* after initialization through pthread\_mutexattr\_init() function. Mutexes created with this attribute can only be operated upon by threads within the same process that initialized the mutex. In contrast, mutexes created with PTHREAD\_PROCESS\_SHARED attribute can be shared between any threads that have access to the memory containing the mutex.
&nbsp;
Calling pthread\_mutexattr\_setpshared() if the *attr* is uninitialized results in undefined behavior.

## Returns

- **0**
Success.
&nbsp;
- **EINVAL**
Invalid *pshared* value.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
