# tcgetpgrp()
Get the foreground process group ID

## Synopsis
```c
#include <unistd.h>

pid_t tcgetpgrp(int fd);
```

## Arguments

- **fd**
The file descriptor associated with the terminal queried.

## Library
libc

## Description

The tcgetpgrp() function returns the value of the process group ID of the foreground process group of the terminal associated with *fd*. If there is no foreground process group, a value greater than one which is not equal to the process group ID of any existing process group will be returned.
&nbsp;

This function can be called by a member process of a background process group. But the information may be changed by a member of the foreground process group later.

## Returns

- **pgid**
The process group ID of the foreground process group will be returned upon success.
&nbsp;

- **-1**
An error occurred.

## Errors

- **EBADF**
The *fd* argument is not a valid file descriptor.
&nbsp;

- **ENOTTY**
The calling process does not have a controlling terminal, or the file is not associated with the controlling terminal of the calling process.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
