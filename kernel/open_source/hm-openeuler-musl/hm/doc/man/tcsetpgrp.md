# tcsetpgrp()
Set foreground process group ID

## Synopsis
```c
#include <unistd.h>

int tcsetpgrp(int fd, pid_t pgrp);
```

## Arguments

- **fd**
A file descriptor associated with the controlling terminal of the calling process.
&nbsp;
- **pgrp**
A process group ID of a process in the same session as the calling process.


## Library
libc

## Description
The tcsetpgrp() function sets foreground process group ID, the controlling terminal must be currently associated with the session of the calling process.

## Returns

- **0**
Successfully set foreground process group ID.
&nbsp;

- **-1**
An error occurred.

## Errors

- **EBADF**
The *fd* argument is not a valid file descriptor.
&nbsp;
- **EINVAL**
This implementation does not support the value in the *pgrp* argument.
&nbsp;
- **ENOTTY**
The file associated with *fd* is not a terminal.
&nbsp;
- **EPERM**
The value of *pgrp* is a value supported by the implementation, but does not match the process group ID of a process in the same session as the calling process.
&nbsp;
- **ESRCH**
The value of *pgrp* is a value supported by the implementation, but there is no process with this process ID.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
