# sem\_wait(), sem\_trywait()
Lock a semaphore

## Synopsis
```c
#include <semaphore.h>

int sem_wait(sem_t *sem);
int sem_trywait(sem_t *sem);
```

## Arguments

- **sem**
The pointer to the semaphore.

## Library
libc

## Description
The sem\_wait() function will decrease the semaphore by one. If the semaphore is greater than zero, the call will return immediately. If the semaphore is less than or equal to zero, the calling thread will get blocked until it is possible to perform the decrement. If a blocked thread gets a signal when waiting for the semaphore, it will be interrupted and EINTR will be returned.
&nbsp;
The sem\_trywait() function is equivalent to sem\_wait(), except that sem\_trywait() will not get blocked and return EAGAIN as errno immediately if the semaphore is less than or equal to zero.

## Returns

- **0**
Success.
&nbsp;
- **-1**
An error occurred.

## Errors

- **EINTR**
This function was interrupted by a signal.
&nbsp;
The sem\_trywait() shall return:
&nbsp;
- **EAGAIN**
The semaphore is less than or equal to zero.

## Example
N/A

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
