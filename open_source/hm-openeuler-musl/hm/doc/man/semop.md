# semop()
System V semaphore operations

## Synopsis
```c
#include <sys/sem.h>

int semop(int semid, struct sembuf *sops, size_t nsops);
```

## Arguments

- **semid**
Indicate the semaphore set.
&nbsp;
- **sops**
Pointer to a set of semaphore operations.
&nbsp;
- **nsops**
Number of semaphore operations.

## Library
libc

## Description
The semop() function performs operations on selected semaphores in the set indicated by *semid*.  Each of the *nsops* elements in the array pointed to by *sops* is a structure that specifies an operation to be performed on a single semaphore.

## Returns

- **0**
Success.
&nbsp;
- **-1**
An error occurred.

## Errors

- **E2BIG**
The argument *nsops* is greater than SEMOPM, the maximum number of operations allowed per system call.
&nbsp;
- **EACCES**
The calling process does not have the permissions required to perform the specified semaphore operations.
&nbsp;
- **EAGAIN**
An operation could not proceed immediately and either IPC_NOWAIT was specified in sem_flg.
&nbsp;
- **EFAULT**
An address specified in either the *sops* argument isn't accessible.
&nbsp;
- **EFBIG**
For some operation the value of *sem_num* is less than 0 or greater than or equal to the number of semaphores in the set.
&nbsp;
- **EIDRM**
The semaphore set was removed.
&nbsp;
- **EINTR**
While blocked in this system call, the thread caught a signal.
&nbsp;
- **EINVAL**
The semaphore set doesn't exist, or *semid* is less than zero, or *nsops* has a nonpositive value.
&nbsp;
- **ENOMEM**
The *sem_flg* of some operation specified SEM_UNDO and the system does not have enough memory to allocate the undo structure.
&nbsp;
- **ERANGE**
For some operation *sem_op+semval* is greater than SEMVMX, the implementation dependent maximum value for *semval*.

## Example
&nbsp;

## Classification
 POSIX.1-2001, POSIX.1-2008.

## Function Safety
TODO

## NOTES
This API is not enabled when system is compiled with CONFIG_SYSVIPC off. Calling the API under this condition will return ENOSYS.
