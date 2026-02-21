# sem\_close()
Close a named semaphore

## Synopsis
```c
#include <semaphore.h>

int sem_close(sem_t *sem);
```

## Arguments

- **sem**
The pointer to the semaphore.

## Library
libc

## Description
The sem\_close() function closes the semaphore pointed to by *sem*. The resources for the semaphore allocated to the calling process will be freed.

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
