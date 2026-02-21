# lockf()
Lock, unlock or test lock on an opened file

## Synopsis
```c
#include <unistd.h>

int lockf(int fd, int cmd, off_t len);
```
## Arguments

- **fd**
The file descriptor of the opened file.
&nbsp;
- **cmd**
Lock, unlock or test lock on the file, see description below.
&nbsp;
- **len**
Range in file from current position. If *len* is positive, the range is [pos, pos+len-1]; if *len* is negative, the range is [pos-len, pos-1]; if *len* is 0, the range is [pos, EOF].

## Library
libc

## Description
The function lockf() provides advisory file lock operations on an opened file. Advisory file lock is not mandatory, which means only if a task acquires lock explicitly, the task is impacted by the lock.
&nbsp;
The *cmd* is one of the following:
- **F_LOCK**
Apply a exclusive lock. If another process has already holds a lock, the new one will be blocked.
&nbsp;
- **F_TLOCK**
Apply an exclusive lock but in nonblock mode.
&nbsp;
- **F_ULOCK**
Unlock a lock.
&nbsp;
- **F_TEST**
Check if the lock will be blocked. If not, return 0, otherwise return -1 and errno is set as EAGAIN.
&nbsp;

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
The *cmd* is invalid.
&nbsp;
- **EINTR**
Blocked lock is interrupted by signals.
&nbsp;
- **ENOLCK**
Cannot get internal lock structures.
&nbsp;
- **EAGAIN**
The F\_TLOCK is specified, and cannot lock file.
&nbsp;
- **ENOMEM**
There is not enough memory during the function (unlikely to happen).
&nbsp;
- **EDEADLCK**
A deadlock is detected.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO

## NOTES
This API is not enabled when system is compiled with CONFIG\_FS\_FLOCK off. Calling the API under this condition will return ENOSYS.
