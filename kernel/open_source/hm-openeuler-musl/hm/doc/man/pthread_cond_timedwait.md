# pthread\_cond\_timedwait()
Wait on a condition variable until the specified time

## Synopsis
```c
#include <pthread.h>

int pthread_cond_timedwait(pthread_cond_t *cond,
                           pthread_mutex_t *mutex,
                           const struct timespec *abstime);
```

## Arguments

- **cond**
A pointer to the condition variable.
&nbsp;
- **mutex**
A pointer to the mutex that works with the condition variable.
&nbsp;
- **abstime**
The absolute time until when the calling thread terminates waiting on the condition variable.

## Library
libc

## Description
The behavior of pthread\_cond\_timedwait() is mostly equivalent to pthread\_cond\_wait() except that an error is returned if the absolute time specified by *abstime* passes before the blocked calling thread is awakened by pthread\_cond\_signal() or pthread\_cond\_broadcast(), or if the absolute time specified by *abstime* has already been passed at the time of the call.

## Returns

- **0**
Success.
&nbsp;
- **EPERM**
The mutex type is PTHREAD\_MUTEX\_RECURSIVE or PTHREAD\_MUTEX\_ERRORCHECK, or the mutex is a robust mutex, but the current thread is not the owner of the mutex.
&nbsp;
- **EINVAL**
The *abstime* argument specified a nanosecond value that is less than zero or equal to or greater than 1 billion.
&nbsp;
- **ETIMEDOUT**
The absolute time specified by *abstime* has passed.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
