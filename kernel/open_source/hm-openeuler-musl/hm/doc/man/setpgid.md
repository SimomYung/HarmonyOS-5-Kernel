# setpgid()
Set process group ID for job control

## Synopsis
```c
#include <unistd.h>

int setpgid(pid_t pid, pid_t pgid);
```

## Arguments

- **pid**
The process ID of the process whose process group ID is to be changed.
&nbsp;
- **pgid**
The process group ID to be set.

## Library
libc

## Description

The setpgid() function either joins an existing process group or creates a new process group in the same session of the calling process. The process group ID of a session leader will not be changed. If setpgid() succeeds, the process group ID of the process specified by *pid* will be set to pgid.

If *pid* or *pgid* is zero, the process ID of the calling process will be used.

## Returns

- **0**
The setpgid() function succeeds.
&nbsp;

- **-1**
An error occurred.

## Errors

- **EACCES**
The argument *pid* matches the process ID of a child process of the calling process and the child process has executed exec functions before.
&nbsp;
- **EINVAL**
The argument *pgid* is less than zero or a unsupported value.
&nbsp;
- **EPERM**
The process specified by *pid* is a session leader. Or *pid* matches a child process of the calling process and the child process is not in the same session as the calling process. Or *pgid* is valid but does not match *pid* and there is not a process group whose process group ID is equal to *pgid* in the same session.
&nbsp;
- **ESRCH**
The argument *pid* is not equal to the process ID of the calling process or any child process of the calling process.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
