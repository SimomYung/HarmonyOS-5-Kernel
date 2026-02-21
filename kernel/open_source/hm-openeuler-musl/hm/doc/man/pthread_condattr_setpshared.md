# pthread\_condattr\_setpshared()
Set the process-shared condition variable attribute

## Synopsis
```c
#include <pthread.h>

int pthread_condattr_setpshared(pthread_condattr_t *attr, int pshared);
```

## Arguments

- **attr**
A pointer to the condition variable attributes object.
&nbsp;
- **pshared**
The process-shared condition variable attribute value to be set.

## Library
libc

## Description
The pthread\_condattr\_setpshared() function sets the process-shared condition variable attribute specified by *pshared* in *attr*. Valid values are PTHREAD\_PROCESS\_PRIVATE and PTHREAD\_PROCESS\_SHARED.
&nbsp;
PTHREAD\_PROCESS\_PRIVATE is the default value in *attr* after initialization through pthread\_condattr\_init() function. Condition variables created with this attribute can only be operated upon by threads within the same process that initialized the condition variable. In contrast, condition variables created with PTHREAD\_PROCESS\_SHARED attribute can be shared between any threads that have access to the memory containing the condition variable.

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
