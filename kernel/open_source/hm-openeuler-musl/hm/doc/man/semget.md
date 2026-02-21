# semget()
Get a System V semaphore set identifier

## Synopsis
```c
#include <sys/sem.h>

int semget(key_t key, int nsems, int semflg);
```

## Arguments

- **key**
Key value that used to create or open a System V semaphore array.
&nbsp;
- **nsems**
The number of semaphores in the semaphore array to create.
&nbsp;
- **semflg**
Flags that control the operation of the call.

## Library
libc

## Description
The semget() function returns the System V semaphore set identifier associated with the argument *key*.  It may be used either to obtain the identifier of a previously created semaphore set (when *semflg* is zero and *key* does not have the value IPC_PRIVATE), or to create a new set.

## Returns

- **A semaphore set descriptor**
Success.
&nbsp;
- **-1**
An error occurred.

## Errors

- **EACCES**
A semaphore set exists for *key*, but the calling process does not have permission to access the set.
&nbsp;
- **EEXIST**
IPC_CREAT and IPC_EXCL were specified in *semflg*, but a semaphore set already exists for *key*.
&nbsp;
- **EINVAL**
*nsems* is less than 0 or greater than the limit on the number of semaphores per semaphore set (SEMMSL).
&nbsp;
- **EINVAL**
A semaphore set corresponding to *key* already exists, but *nsems* is larger than the number of semaphores in that set.
&nbsp;
- **ENOENT**
No semaphore set exists for *key* and *semflg* did not specify IPC_CREAT.
&nbsp;
- **ENOMEM**
A semaphore set has to be created but the system does not have enough memory for the new data structure.
&nbsp;
- **ENOSPC**
A semaphore set has to be created but the system limit for the maximum number of semaphore sets (SEMMNI), or the system wide maximum number of semaphores (SEMMNS), would be exceeded.

## Example
&nbsp;

## Classification
 POSIX.1-2001, POSIX.1-2008.

## Function Safety
TODO

## NOTES
This API is not enabled when system is compiled with CONFIG_SYSVIPC off. Calling the API under this condition will return ENOSYS.
