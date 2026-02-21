# pthread\_rwlock\_rdlock(), pthread\_rwlock\_tryrdlock()
Lock a read-write lock for reading

## Synopsis
```c
#include <pthread.h>

int pthread_rwlock_rdlock(pthread_rwlock_t *rwlock);
int pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock);
```

## Arguments

- **rwlock**
The pointer to the read-write lock.

## Library
libc

## Description
The pthread\_rwlock\_rdlock() function will apply a read lock on *rwlock*. If *rwlock* is already locked by a writer, the calling thread will get blocked until *rwlock* is unlocked by the writer. The read-write lock has a read-lock counter to record the number of read locks. Each time a thread tries to apply a read lock on the read-write lock which is not hold by a writer, the counter will be increased by one and the call of pthread\_rwlock\_rdlock() will return successfully. Each time the read-write lock is unlocked by a reader, the counter will be decreased by one until the counter reaches zero, in which case the read-write lock will be in unlocked state. The pthread\_rwlock\_rdlock() function will fail if the counter exceeds the maximum number. A deadlock will happen if a thread calls pthread\_rwlock\_rdlock() on the read-write lock which has been hold by itself as writer. If a blocked thread gets a signal when waiting for a read-write lock, it will resume waiting after handling the signal as if it is not interrupted.
&nbsp;
The pthread\_rwlock\_tryrdlock() function is equivalent to pthread\_rwlock\_rdlock(), except that pthread\_rwlock\_tryrdlock() will not get blocked and return EBUSY immediately if *rwlock* is locked by a writer.
&nbsp;
Calling pthread\_rwlock\_rdlock() or pthread\_rwlock\_tryrdlock() if *rwlock* is uninitialized results in undefined behavior.

## Returns

- **0**
Success.
&nbsp;
- **EAGAIN**
The number of read locks on *rwlock* has been exceeded.
&nbsp;
The pthread\_rwlock\_tryrdlock() shall return:
&nbsp;
- **EBUSY**
*rwlock* is hold by a writer.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
