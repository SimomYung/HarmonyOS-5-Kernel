# pthread\_attr\_setinheritsched()
Set the inherit-scheduler attribute in the thread attributes object.

## Synopsis
```c
#include <pthread.h>

int pthread_attr_setinheritsched(pthread_attr_t *attr, int inheritsched);
```

## Arguments

- **attr**
A pointer to the thread attributes object.
&nbsp;
- **inheritsched**
The inherit-scheduler attribute to be set, can be PTHREAD\_INHERIT\_SCHED or PTHREAD\_EXPLICIT\_SCHED.

## Library
libc

## Description
The pthread\_attr\_setinheritsched() function sets the inherit scheduler value specified by *inheritsched* to the thread attributes object specified by *attr*. Valid values are PTHREAD\_INHERIT\_SCHED or PTHREAD\_EXPLICIT\_SCHED.
&nbsp;
PTHREAD\_INHERIT\_SCHED means all threads created with *attr* shall inherit scheduling policy and priority from the caller, any scheduling attributes set in *attr* will be ignored.
&nbsp;
PTHREAD\_EXPLICIT\_SCHED means all threads created with *attr* shall take scheduling policy and priority from *attr*.
&nbsp;
Following functions can be used to set scheduling attributes in *attr*:&nbsp;
pthread\_attr\_setschedparam()&nbsp;
pthread\_attr\_setschedpolicy()

## Returns

- **0**
Success.
&nbsp;
- **EINVAL**
The parameter *inheritsched* is invalid.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
