# pthread\_cond\_wait()
Wait on a condition variable

## Synopsis
```c
#include <pthread.h>

int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);
```

## Arguments

- **cond**
A pointer to the condition variable.
&nbsp;
- **mutex**
A pointer to the mutex that works with the condition variable.

## Library
libc

## Description
The pthread\_cond\_wait() function makes the calling thread blocked on the condition variable specified by *cond*. Make sure that the calling thread has already held the mutex specified by *mutex* before calling pthread\_cond\_wait(), otherwise, you may get unexpected results.
&nbsp;
This function will release *mutex* and cause the calling thread to block on the condition variable *cond*. Upon successful return, the mutex will be locked and owned by the calling thread again.
&nbsp;
If a signal is delivered to the thread that is currently blocked on the condition variable, upon the return from the signal handler, the thread will continue to wait for the condition variable as if it was not interrupted.

## Returns

- **0**
Success.
&nbsp;
- **EPERM**
The mutex type is PTHREAD\_MUTEX\_RECURSIVE or PTHREAD\_MUTEX\_ERRORCHECK, or the mutex is a robust mutex, but the current thread is not the owner of the mutex.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
