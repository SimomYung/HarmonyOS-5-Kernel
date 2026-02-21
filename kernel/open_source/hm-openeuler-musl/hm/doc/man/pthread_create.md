# pthread\_create()
Create a new thread

## Synopsis
```c
#include <pthread.h>

int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                   void *(*start_routine) (void *), void *arg);
```

## Arguments

- **thread**
A pointer to the buffer which stores the thread descriptor of the new thread.
&nbsp;
- **attr**
Thread attributes for the new thread.
&nbsp;
- **start_routine**
Entry function for the new thread.
&nbsp;
- **arg**
Argument needed by the entry function.

## Library
libc

## Description
The pthread\_create() functions creates a new thread with the attribute specified by *attr*, the new created thread will start from *start_routine* with *arg* as single argument. If the *start_routine* returns, an implicit pthread\_exit() is called using the return value of *start_routine* as the exit status.
&nbsp;
*attr* should be initialized with pthread\_attr\_init(), multiple attributes can be set using pthread\_attr\_set prefixed functions. If *attr* is NULL, then default attributes will be used.
&nbsp;
A successful call to pthread\_create() stores the descriptor of the new thread in the buffer pointed to by *thread*, this descriptor can be used to refer to the new thread in subsequent calls to other pthreads functions.

## Returns

- **0**
Success.
&nbsp;
- **EINVAL**
Invalid settings in *attr* (including but not limited to scheduling policy, scheduling priority and affinity).
&nbsp;
- **EAGAIN**
The system lacked the necessary resources to create another thread.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
