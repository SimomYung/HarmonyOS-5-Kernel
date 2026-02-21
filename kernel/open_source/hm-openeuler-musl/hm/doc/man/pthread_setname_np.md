# pthread\_setname\_np()
Set thread name

## Synopsis
```c
#define _GNU_SOURCE
#include <pthread.h>

int pthread_setname_np(pthread_t thread, const char *name);
```

## Arguments

- **thread**
The thread descriptor whose name is to be set.
&nbsp;
- **name**
The pointer of the buffer containing the thread name.

## Library
libc

## Description
The pthread\_setname\_np() function can be used to set the name of a thread. By default, all threads created by pthread\_create() inherit the name of the program. The name set to the thread must be a C language string, and the length of it must be no longer than 16 characters(include '\\0').
&nbsp;
Note that if the resource of the target thread is releasing(which means that the thread is detached and exiting, or the thread has exited and the joiner is working on the resource releasing), calling this function may lead to unknown behavior in this concurrency scenarios.

## Returns

- **0**
Success.
&nbsp;
- **ERANGE**
The length of *name* is longer than allowed limit.
&nbsp;
- **ESRCH**
The thread specified by *thread* is not found.
&nbsp;
If pthread\_setname\_np() function fails to open or write /proc/self/task/[tid]/comm, the call will return the errors of open() or write().

## Errors
N/A

## Example
&nbsp;

## Classification
Unix

## Function Safety
TODO
