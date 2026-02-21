# sem\_destroy()
Destroy an unnamed semaphore

## Synopsis
```c
#include <semaphore.h>

int sem_destroy(sem_t *sem);
```

## Arguments

- **sem**
The pointer to the semaphore.

## Library
libc

## Description
The sem\_destroy() function destroys an unnamed semaphore pointed to by *sem*.
&nbsp;
Destroying a semaphore that other threads are blocked on results in undefined behavior. Referencing a destroyed semaphore results in undefined behavior, unless it is reinitialized using sem\_init(). Destroying a semaphore which is not initialized results in undefined behavior.

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
