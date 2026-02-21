# pthread\_mutex\_get\_owner()
Return the owner tid of the mutex

## Synopsis
```c
#include <hmpthread.h>

pid_t pthread_mutex_get_owner(const pthread_mutex_t *mutex);
```

## Arguments

- **mutex**
The pointer to the mutex.

## Library
libc

## Description

The pthread\_mutex\_get\_owner() function will return the tid of the thread who owns the mutex, and return 0 if the mutex is in unlocked state.

## Returns

- **0**
The mutex is in unlocked state.
&nbsp;
- **tid**
The tid of the thread who owns the mutex.
&nbsp;
- **-1**
An error occurred.

## Errors

- **EINVAL**
The mutex is NULL.
&nbsp;
- **ENOTRECOVERABLE**
The mutex is not recoverable.

## Example
N/A

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
