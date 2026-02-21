# pthread\_rwlock\_wrlock(), pthread\_rwlock\_trywrlock()
Lock a read-write lock for writing

## Synopsis
```c
#include <pthread.h>

int pthread_rwlock_wrlock(pthread_rwlock_t *rwlock);
int pthread_rwlock_trywrlock(pthread_rwlock_t *rwlock);
```

## Arguments

- **rwlock**
The pointer to the read-write lock.

## Library
libc

## Description
The pthread\_rwlock\_wrlock() function will apply a write lock on *rwlock*. If *rwlock* is already locked by other thread no matter for reading or writing, the calling thread will get blocked until no other thread holds *rwlock* anymore. A deadlock will happen if a thread calls pthread\_rwlock\_wrlock() on a read-write lock which has been hold by itself. If a blocked thread gets a signal when waiting for a read-write lock, it will resume waiting after handling the signal as if it is not interrupted.
&nbsp;
The pthread\_rwlock\_trywrlock() function is equivalent to pthread\_rwlock\_wrlock(), except that pthread\_rwlock\_trywrlock will not get blocked and return EBUSY immediately if *rwlock* is hold by other thread.
&nbsp;
Calling pthread\_rwlock\_wrlock() or pthread\_rwlock\_trywrlock() if *rwlock* is uninitialized results in undefined behavior.

## Returns

- **0**
Success.
&nbsp;
The pthread\_rwlock\_trywrlock() shall return:
&nbsp;
- **EBUSY**
*rwlock* is hold by other thread.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
