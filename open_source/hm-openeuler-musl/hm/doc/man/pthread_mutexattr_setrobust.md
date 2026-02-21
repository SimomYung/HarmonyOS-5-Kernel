# pthread\_mutexattr\_setrobust(), pthread\_mutexattr\_setrobust\_np()
Set the mutex robust attribute

## Synopsis
```c
#include <pthread.h>

int pthread_mutexattr_setrobust(const pthread_mutexattr_t *attr, int robust);
int pthread_mutexattr_setrobust_np(const pthread_mutexattr_t *attr,
                                   int robust);
```

## Arguments

- **attr**
A pointer to the mutex attributes object.
&nbsp;
- **robust**
Robust attribute value to be set.

## Library
libc

## Description
The pthread\_mutexattr\_setrobust() function sets the robust attribute value indicated by *robust* in *attr*. Valid values are PTHREAD\_MUTEX\_STALLED and PTHREAD\_MUTEX\_ROBUST.
&nbsp;
PTHREAD\_MUTEX\_STALLED is the default value in *attr* after initialization through pthread\_mutexattr\_init() function. If a thread dies holding a mutex of this type, the mutex will remain locked, and any other attempts to call pthread\_mutex\_lock() on the mutex will block. This can lead to deadlocks if no other thread can unlock the mutex.
&nbsp;
The mutex of type PTHREAD\_MUTEX\_ROBUST is called robust mutex. If a thread dies holding a robust mutex, then the next thread that acquires the mutex will fail with error EOWNERDEAD. By checking the return value of the lock function, we can be informed of the termination event. Usually after EOWNERDEAD is returned, calling pthread\_mutex\_consistent() (make the robust mutex in the consistent state) and pthread\_mutex\_unlock() (unlock the mutex) can make the robust mutex be available again. If the dead mutex is unlocked without a call to pthread\_mutex\_consistent(), it will be in an unusable state forever and any other attempts to lock the mutex will fail with error ENOTRECOVERABLE.
&nbsp;
The pthread\_mutexattr\_setrobust\_np() function is equivalent to pthread\_mutexattr\_setrobust(). It is introduced by glibc before the addition of pthread\_mutexattr\_setrobust() to POSIX.
&nbsp;
Calling pthread\_mutexattr\_setrobust() or pthread\_mutexattr\_setrobuse\_np() if the *attr* is uninitialized results in undefined behavior.

## Returns

- **0**
Success.
&nbsp;
- **EINVAL**
Invalid *robust* value.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
