# pthread\_attr\_getinheritsched()
Set the inherit-scheduler attribute in the thread attributes object

## Synopsis
```c
#include <pthread.h>

int pthread_attr_getinheritsched(const pthread_attr_t *restrict attr, int *restrict inherit)
```

## Arguments

- **attr**
A pointer to the thread attributes object.
&nbsp;
- **inherit**
A pointer to the buffer that stores inherit-scheduler, which can be PTHREAD\_INHERIT\_SCHED or PTHREAD\_EXPLICIT\_SCHED.

## Library
libc

## Description
The pthread\_attr\_getinheritsched() returns the inherit-scheduler attribute of the thread attributes object attr in the buffer pointed to by inheritsched.
&nbsp;
PTHREAD\_INHERIT\_SCHED means all threads created with *attr* shall inherit scheduling policy and priority from the caller, any scheduling attributes set in *attr* will be ignored.
&nbsp;
PTHREAD\_EXPLICIT\_SCHED means all threads created with *attr* shall take scheduling policy and priority from *attr*.
&nbsp;
Following functions can be used to set scheduling attributes in *attr*:
pthread\_attr\_setschedparam()
pthread\_attr\_setschedpolicy()

## Returns

- **0**
Success.
&nbsp;

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
