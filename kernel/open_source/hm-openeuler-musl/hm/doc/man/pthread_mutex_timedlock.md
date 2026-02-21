# pthread\_mutex\_timedlock()
Lock a mutex with a time limit

## Synopsis
```c
#include <pthread.h>
#include <time.h>

int pthread_mutex_timedlock(pthread_mutex_t *restrict mutex,
                            const struct timespec *restrict abstime);
```

## Arguments

- **mutex**
The pointer to the mutex.
&nbsp;
- **abstime**
The maximum time to wait on the mutex, in absolute time.

## Library
libc

## Description
The pthread\_mutex\_timedlock() function will return with the unlocked mutex in locked state and the mutex owner will be set to the calling thread. If the mutex is already locked by other thread, the calling thread will get blocked until the mutex is unlocked by its owner or the timeout specified by *abstime* expires. If the *mutex* is available before the timeout expires, the call of pthread\_mutex\_timedlock() will lock the *mutex*(or increase the lock counter in case of the PTHREAD\_MUTEX\_RECURSIVE mutex) successfully. Otherwise, it will fail and ETIMEDOUT will be returned. *abstime* should be based on CLOCK\_REALTIME clock. If the mutex can be acquired at the time of the call, the validity of *abstime* will not be checked.
&nbsp;
If the owner of a robust mutex terminated before unlock the robust mutex, a call to pthread\_mutex\_timedlock() will lock the mutex and EOWNERDEAD will be returned, but the mutex will be marked as inconsistent. It should be ensured that pthread\_mutex\_consistent() is called to recover this robust mutex consistent again. If it failed to recover, the mutex will be unusable permanently after being unlocked.
&nbsp;
If a blocked thread gets a signal when waiting for a mutex, it will resume waiting after handling the signal as if it is not interrupted.
&nbsp;
Calling pthread\_mutex\_timedlock() if the mutex is uninitialized results in undefined behavior.

## Returns

- **0**
Success.
&nbsp;
- **EAGAIN**
The mutex type is PTHREAD\_MUTEX\_RECURSIVE and the maximum number of the lock counter has been exceeded.
&nbsp;
- **ENOTRECOVERABLE**
The mutex is not recoverable.
&nbsp;
- **EOWNERDEAD**
The mutex is a robust mutex and the owner terminates before unlocking the mutex. The calling thread will acquire the mutex and should make the mutex state consistent.
&nbsp;
- **EDEADLK**
The mutex type is PTHREAD\_MUTEX\_ERRORCHECK and the owner tries to lock it again, in which condition deadlock happens.
&nbsp;
- **EINVAL**
The value of *abstime* is invalid(the nanosecond value of *abstime* is less than zero, or greater than or equal to 1000 million).
&nbsp;
- **ETIMEDOUT**
The mutex can not be acquired before the timeout expires.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
