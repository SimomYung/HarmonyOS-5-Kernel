# flock()
Lock or unlock an opened file

## Synopsis
```c
#include <sys/file.h>

int flock(int fd, int ops);
```
## Arguments

- **fd**
The file descriptor of the opened file.
&nbsp;
- **ops**
Lock or unlock operation on the file, see description below.

## Library
libc

## Description
The function flock() provides advisory file lock operations on an opened file. Advisory file lock is not mandatory, which means only if a task acquires lock explicitly, the task is impacted by the lock.
&nbsp;
The operation is one of the following:
- **LOCK_SH**
Apply a shared lock.
&nbsp;
- **LOCK_EX**
Apply an exclusive lock.
&nbsp;
- **LOCK_UN**
Unlock a lock.
&nbsp;
- **LOCK_NB**
Apply a lock in non-block mode, could be OR'ed with LOCK\_SH or LOCK\_EX.
&nbsp;
If an exclusive lock is acquired without LOCK\_UN, the request may get blocked, until the previous lock is unlocked. Otherwise, EAGAIN is returned.

## Returns

- **0**
Function finished successfully.
&nbsp;
- **-1**
An error occurred.

## Errors

- **EBADF**
The input *fd* is invalid.
&nbsp;
- **EACCES**
Operation is denied.
&nbsp;
- **EINVAL**
The *ops* is invalid.
&nbsp;
- **EINTR**
Blocked lock is interrupted by signals.
&nbsp;
- **EAGAIN**
The LOCK\_NB is specified, and cannot lock file.
&nbsp;
- **ENOMEM**
There is not enough memory during the function (unlikely to happen).

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO

## NOTES
This API is not enabled when system is compiled with CONFIG_FS_FLOCK off. Calling the API under this condition will return ENOSYS.
