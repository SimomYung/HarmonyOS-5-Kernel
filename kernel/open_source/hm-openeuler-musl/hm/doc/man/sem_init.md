# sem\_init()
Initialize an unnamed semaphore

## Synopsis
```c
#include <semaphore.h>

int sem_init(sem_t *sem, int pshared, unsigned int value);
```

## Arguments

- **sem**
The pointer to the semaphore.
&nbsp;
- **pshared**
The process-shared mode.
&nbsp;
- **value**
The initial value of the semaphore.

## Library
libc

## Description
The sem\_init() function initializes the unnamed semaphore pointed to by *sem*. The initial value of *sem* will be set to the argument *value*. *pshared* indicates whether *sem* can be shared between processes. If *pshared* is zero, the semaphore can only be used by threads in the same process which the calling thread in. If *pshared* is nonzero, the semaphore can be used by any thread in any process.
&nbsp;
Using an uninitialized semaphore results in undefined behavior. Referencing a destroyed semaphore results in undefined behavior, unless it is reinitialized using sem\_init(). Initializing an already initialized semaphore results in undefined behavior.

## Returns

- **0**
Success.
&nbsp;
- **-1**
An error occurred.

## Errors

- **EINVAL**
*value* exceeds the maximum allowable value.

## Example
N/A

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
