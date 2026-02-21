#  pthread\_rwlock\_destroy()
Destroy a read-write lock

## Synopsis
```c
#include <pthread.h>

int pthread_rwlock_destroy(pthread_rwlock_t *rwlock);
```

## Arguments

- **rwlock**
The pointer to read-write lock.

## Library
libc

## Description
The pthread\_rwlock\_destroy() will destroy the read-write lock, meaning the read-write lock is no longer usable.
&nbsp;
Destroying a read-write lock held by other thread results in undefined behavior. Referencing a destroyed read-write lock results in undefined behavior, unless it is reinitialized using pthread\_rwlock\_init(). Using copies of read-write lock in calls to pthread\_rwlock\_rdlock(), pthread\_rwlock\_wrlock(), pthread\_rwlock\_tryrdlock(), pthread\_rwlock\_trywrlock(), pthread\_rwlock\_timedrdlock(), pthread\_rwlock\_timedwrlock(), pthread\_rwlock\_unlock(), pthread\_rwlock\_destroy() results in undefined behavior. Destroying a read-write lock which is not initialized results in undefined behavior.

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
