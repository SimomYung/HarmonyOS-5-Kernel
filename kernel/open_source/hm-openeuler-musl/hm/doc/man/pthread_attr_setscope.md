# pthread\_attr\_setscope()
Set the contention scope attribute in the thread attributes object.

## Synopsis
```c
#include <pthread.h>

int pthread_attr_setscope(pthread_attr_t *attr, int scope);
```

## Arguments

- **attr**
A pointer to the thread attributes object.
&nbsp;
- **scope**
Contention scope value, only PTHREAD\_SCOPE\_SYSTEM is supported now.

## Library
libc

## Description
The pthread\_attr\_setscope() function sets the contention scope value specified by *scope* to the thread attributes object specified by *attr*. This attribute defines in which scope a thread competes resources such as CPU with other threads.
&nbsp;
The only supported value is PTHREAD\_SCOPE\_SYSTEM, which means new thread created with *attr* competes resources against all other threads system-wide.

## Returns

- **0**
Success.
&nbsp;
- **ENOTSUP**
The parameter *scope* is PTHREAD\_SCOPE\_PROCESS.
&nbsp;
- **EINVAL**
Invalid contention scope value.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
