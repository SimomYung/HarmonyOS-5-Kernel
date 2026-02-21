# shmdt()
System V shared memory operations

## Synopsis
```c
#include <sys/shm.h>

int shmdt(const void *shmaddr);
```

## Arguments

- **shmaddr**
Specify the address from the address space of the calling process.

## Library
libc

## Description
shmdt() detaches the shared memory segment located at the address specified by *shmaddr* from the address space of the calling process. The to-be-detached segment must be currently attached with *shmaddr* equal to the value returned by the attaching shmat() call.

## Returns

- **0**
Success.
&nbsp;
- **-1**
An error occurred.

## Errors

- **EINVAL**
There is no shared memory segment attached at *shmaddr*; or, *shmaddr* is not aligned on a page boundary.

## Example
&nbsp;

## Classification
POSIX.1-2001, POSIX.1-2008, SVr4.

## Function Safety
TODO

## NOTES
This API is not enabled when system is compiled with CONFIG_SYSVIPC off. Calling the API under this condition will return ENOSYS.
