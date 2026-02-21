# sem\_post()
Unlock a semaphore

## Synopsis
```c
#include <semaphore.h>

int sem_post(sem_t *sem);
```

## Arguments

- **sem**
The pointer to the semaphore.


## Library
libc

## Description
The sem\_post() function increases the semaphore by one. If there are some blocked threads waiting for the semaphore, one of them will be woken up to acquire the semaphore.

## Returns

- **0**
Success.
&nbsp;
- **-1**
An error occurred.

## Errors

- **EOVERFLOW**
The semaphore will exceed the maximum allowable value.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
