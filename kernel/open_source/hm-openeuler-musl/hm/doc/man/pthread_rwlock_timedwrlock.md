# pthread\_rwlock\_timedwrlock()
Lock a read-write lock for writing with a time limit

## Synopsis
```c
#include <pthread.h>
#include <time.h>

int pthread_rwlock_timedwrlock(pthread_rwlock_t *restrict rwlock,
                               const struct timespec *restrict abstime);
```

## Arguments

- **rwlock**
The pointer to the read-write lock.
&nbsp;
- **abstime**
The maximum time to wait on the read-write lock, in absolute time.

## Library
libc

## Description
The pthread\_rwlock\_timedwrlock() function will apply a write lock on *rwlock*. If *rwlock* is already locked by other thread no matter for reading or writing, the calling thread will get blocked until no thread holds *rwlock* or the timeout specified by *abstime* expires. If *rwlock* is available before the timeout expires, the call of pthread\_rwlock\_timedwrlock() will apply a write lock on *rwlock* successfully. Otherwise, the call will fail and ETIMEDOUT will be returned. *abstime* should be based on CLOCK\_REALTIME clock. If *rwlock* can be acquired at the time of the call, the validity of *abstime* will not be checked. If a blocked thread gets a signal when waiting for a read-write lock, it will resume waiting after handling the signal as if it is not interrupted.
&nbsp;
Calling pthread\_rwlock\_timedwrlock() if *rwlock* is uninitialized results in undefined behavior.

## Returns

- **0**
Success.
&nbsp;
- **EINVAL**
The value of *abstime* is invalid(the nanosecond value of *abstime* is less than zero, or greater than or equal to 1000 million).
&nbsp;
- **ETIMEDOUT**
*rwlock* can not be acquired before the timeout expires.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
