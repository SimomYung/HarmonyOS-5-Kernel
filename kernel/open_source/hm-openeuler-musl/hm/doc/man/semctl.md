# semctl()
System V semaphore control operations

## Synopsis
```c
#include <sys/sem.h>

int semctl(int semid, int semnum, int cmd, ...);
```

## Arguments

- **semid**
Indicate the semaphore set.
&nbsp;
- **semnum**
The semnum-th semaphore of the semaphore set.
&nbsp;
- **cmd**
Semctl operation commands.

## Library
libc

## Description
The semctl() function performs the control operation specified by *cmd* on the System V semaphore set identified by *semid*, or on the semnum-th semaphore of that set.  (The semaphores in a set are numbered starting at 0.) This function has three or four arguments, depending on 'cmd'.  When there are four, the fourth argument is 'arg', it stores the control or query information of the semaphore set.
&nbsp;
Currently we support IPC_RMID, SETVAL, SETALL, GETVAL, GETALL, GETPID, GETZCNT, GETNCNT, IPC_STAT and SEM_STAT operation commands.
&nbsp;
IPC_INFO, SEM_INFO, SEM_STAT_ANY are not supported.

## Returns

On Success, the semctl() function returns the value of semncnt when semctl flag is GETNCNT, returns the value of sempid when semctl flag is GETPID, returns value of semval when semctl flag is GETVAL, returns the value of semzcnt when semctl flag is GETZCNT, returns the index of the highest used entry in the kernel's internal array recording information about all semaphore sets when semctl flag is IPC_INFO or SEM_INFO, returns the identifier of the semaphore set whose index was given in *semid* when semctl flag is SEM_STAT or SEM_STAT_ANY.
&nbsp;
On error, -1 is returned with *errno* indicating the error.

## Errors

- **EACCES**
The argument *cmd* has one of the values GETALL, GETPID, GETVAL, GETNCNT, GETZCNT, IPC_STAT, SEM_STAT, SEM_STAT_ANY, SETALL or SETVAL and the calling process does not have the required permissions on the semaphore set.
&nbsp;
- **EFAULT**
The address pointed to by *arg.buf* or *arg.array* isn't accessible.
&nbsp;
- **EIDRM**
Try to use the semaphore set but the semaphore set is being deleted but not completely.
&nbsp;
- **EINVAL**
Invalid value for *cmd* or *semid*. Or: try to use the semaphore set but the semaphore set has been deleted completely.
&nbsp;
- **EPERM**
The argument *cmd* has the value IPC_SET or IPC_RMID but the effective user ID of the calling process is not the creator (as found in *sem_perm.cuid*) or the owner (as found in *sem_perm.uid*) of the semaphore set.
&nbsp;
- **ERANGE**
The argument *cmd* has the value SETALL or SETVAL and the value to which semval is to be set (for some semaphore of the set) is less than 0 or greater than the implementation limit SEMVMX.

## Example
&nbsp;

## Classification
 POSIX.1-2001, POSIX.1-2008.

## Function Safety
TODO

## NOTES
This API is not enabled when system is compiled with CONFIG_SYSVIPC off. Calling the API under this condition will return ENOSYS.
