# pthread\_rwlock\_unlock()
Unlock a read-write lock

## Synopsis
```c
#include <pthread.h>

int pthread_rwlock_unlock(pthread_rwlock_t *rwlock);
```

## Arguments

- **rwlock**
The pointer to the read-write lock.

## Library
libc

## Description
If *rwlock* is in write lock state, the pthread\_rwlock\_unlock() function will set *rwlock* in unlock state. If *rwlock* is in read lock state, the pthread\_rwlock\_unlock() function will decrease the read-lock counter by one, and *rwlock* will remain in read lock state until the read-lock counter reaches zero. If there are blocked threads waiting for *rwlock*, the scheduling policy will determine which one to be chosen to acquire *rwlock* after *rwlock* is available.
&nbsp;
Calling pthread\_rwlock\_unlock() if *rwlock* is uninitialized results in undefined behavior.

## Returns

- **0**
Success.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
