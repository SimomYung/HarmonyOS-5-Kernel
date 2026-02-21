#  pthread\_rwlock\_init()
Initialize a read-write lock

## Synopsis
```c
#include <pthread.h>

int pthread_rwlock_init(pthread_rwlock_t *restrict rwlock,
                        const pthread_rwlockattr_t *restrict attr);
pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;
```

## Arguments

- **rwlock**
The pointer to read-write lock.
&nbsp;
- **attr**
The pointer to attributes using to initialize the rwlock.

## Library
libc

## Description
The pthread\_rwlock\_init() can initialize the read-write lock with attributes specified by *attr*. The read-write lock will get default attributes if *attr* is NULL or *attr* points to a default attributes object. In additional, the macro PTHREAD\_RWLOCK\_INITIALIZER can also give read-write lock default attributes, except that no error checks will be performed. The read-write lock will become initialized and unlocked after successful initialization.
&nbsp;
Using an uninitialized read-write lock results in undefined behavior. Referencing a destroyed read-write lock results in undefined behavior, unless it is reinitialized using pthread\_rwlock\_init(). Using copies of read-write lock in calls to pthread\_rwlock\_rdlock(), pthread\_rwlock\_wrlock(), pthread\_rwlock\_tryrdlock(), pthread\_rwlock\_trywrlock(), pthread\_rwlock\_timedrdlock(), pthread\_rwlock\_timedwrlock(), pthread\_rwlock\_unlock(), pthread\_rwlock\_destroy() results in undefined behavior. Initializing an already initialized read-write lock results in undefined behavior. Passing an uninitialized *attr* argument to pthread\_rwlock\_init() results in undefined behavior.

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
