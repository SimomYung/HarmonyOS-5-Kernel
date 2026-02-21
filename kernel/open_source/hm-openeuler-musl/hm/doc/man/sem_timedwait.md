# sem\_timedwait()
Lock a semaphore

## Synopsis
```c
#include <semaphore.h>

int sem_timedwait(sem_t *__restrict sem, const struct timespec *__restrict abs_timeout);
```

## Arguments

- **sem**
The pointer to the semaphore.
&nbsp;
- **abs_timeout**
The maximum time to wait on the semaphore, in absolute time, the nanosecond value should be [0, 999999999].

## Library
libc

## Description
The sem\_timedwait() function will decrease the semaphore by one. If the semaphore is greater than zero, the call will return immediately. If the semaphore is less than or equal to zero, the calling thread will get blocked until it is possible to perform the decrement or the timeout specified by *abs_timeout* expires. If the decrement can be performed immediately at the time of the call, the validity of *abs_timeout* will not be checked.
&nbsp;
If a blocked thread gets a signal when waiting for the semaphore, it will be interrupted and EINTR will be returned.

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
- **EINVAL**
The value of *abs_timeout* is invalid, or *sem* is invalid.
&nbsp;
- **ETIMEDOUT**
The decrement of the semaphore can not be performed before the timeout expires.

## Example
N/A

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
