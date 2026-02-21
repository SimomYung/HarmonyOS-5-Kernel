#  pthread\_mutex\_destroy()
Destroy a mutex

## Synopsis
```c
#include <pthread.h>

int pthread_mutex_destroy(pthread_mutex_t *mutex);
```

## Arguments

- **mutex**
The pointer to the mutex.

## Library
libc

## Description
The pthread\_mutex\_destroy() will destroy the mutex, meaning the mutex is no longer usable.
&nbsp;
Destroying a locked mutex results in undefined behavior. Referencing a destroyed mutex results in undefined behavior, unless it is reinitialized using pthread\_mutex\_init(). Using copies of mutex in calls to pthread\_mutex\_lock(), pthread\_mutex\_trylock(), pthread\_mutex\_timedlock(), pthread\_mutex\_unlock(), pthread\_mutex\_destroy() results in undefined behavior. Destroying a mutex which is not initialized results in undefined behavior.

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
