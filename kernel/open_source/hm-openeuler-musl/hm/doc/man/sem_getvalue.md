# sem\_getvalue()
Get the value of a semaphore

## Synopsis
```c
#include <semaphore.h>

int sem_getvalue(sem_t *__restrict sem, int *__restrict sval);
```

## Arguments

- **sem**
The pointer to the semaphore.
&nbsp;
- **sval**
The pointer to the memory which stores the value of *sem*.

## Library
libc

## Description
The sem\_getvalue() function gets the value of *sem* and put the value into the memory pointed to by *sval*. If one or more threads are blocked on the semaphore, zero will be returned in *sval*.

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
