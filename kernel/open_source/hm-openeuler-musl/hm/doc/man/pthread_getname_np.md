# pthread\_getname\_np()
Get thread name

## Synopsis
```c
#define _GNU_SOURCE
#include <pthread.h>

int pthread_getname_np(pthread_t thread, char *name, size_t len);
```

## Arguments

- **thread**
The thread descriptor whose name is to be retrieved.
&nbsp;
- **name**
The pointer of the buffer containing the thread name.
&nbsp;
- **len**
The length of the buffer pointed to by *name*.

## Library
libc

## Description
The pthread\_getname\_np() function can be used to get the name of a thread. The buffer length should be at least 16 characters. The returned thread name in the buffer will be '\\0' terminated.
&nbsp;
Note that if the resource of the target thread is releasing(which means that the thread is detached and exiting, or the thread has exited and the joiner is working on the resource releasing), calling this function may lead to unknown behavior in this concurrency scenarios.

## Returns

- **0**
Success.
&nbsp;
- **ERANGE**
The length of name buffer is less than 16 characters.
&nbsp;
If pthread\_getname\_np() function fails to open or read /proc/self/task/[tid]/comm, the call will return the errors of open() or read().

## Errors

- **ESRCH**
The thread specified by *thread* is not found.

## Example
&nbsp;

## Classification
Unix

## Function Safety
TODO
