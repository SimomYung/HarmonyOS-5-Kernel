# pthread\_mutex\_consistent(), pthread\_mutex\_consistent\_np()
Make a robust mutex consistent

## Synopsis
```c
#include <pthread.h>

int pthread_mutex_consistent(pthread_mutex_t *mutex);
int pthread_mutex_consistent_np(pthread_mutex_t *mutex);
```

## Arguments

- **mutex**
The pointer to the mutex.

## Library
libc

## Description
The pthread\_mutex\_consistent() function can make a robust mutex consistent if it is in inconsistent state. A mutex will fall in inconsistent state if the owner of the mutex terminates while holding the mutex. The thread that acquires the inconsistent mutex will lock the inconsistent mutex and get EOWNERDEAD as the return value, then the owner should call pthread\_mutex\_consistent() to make the robust mutex consistent again, otherwise the robust mutex will be unusable permanently after being unlocked.
&nbsp;
The pthread\_mutex\_consistent\_np() function is equivalent to pthread\_mutex\_consistent().
&nbsp;
Calling pthread\_mutex\_consistent() or pthread\_mutex\_consistent\_np() if the *mutex* is uninitialized results in undefined behavior.

## Returns

- **0**
Success.
&nbsp;
- **EINVAL**
The mutex is not robust or is not in inconsistent state or is not in locked state.
&nbsp;
- **EPERM**
The calling thread is not the owner of the robust mutex.

## Errors
N/A

## Example
&nbsp;

## Classification
The pthread\_mutex\_consistent():
POSIX 1003.1-2017
&nbsp;
The pthread\_mutex\_consistent\_np():
Unix

## Function Safety
TODO
