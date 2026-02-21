# killpg()
Send signal to a process group

## Synopsis
```c
#include <signal.h>

int killpg(pid_t pgrp, int sig);
```

## Arguments

- **pgrp**
The ID of the target process group.
&nbsp;
- **sig**
The signum to be sent.

## Library
libc

## Description
The killpg() function sends the signal *sig* to the process group *pgrp*.
&nbsp;
If *pgrp* is 0, the *sig* will be sent to the process group which the calling process belongs to. If *pgrp* is less than 0, the function will return error.

## Returns

- **0**
Signal was sent successfully.
&nbsp;
- **-1**
An error occurred.

## Errors

- **EINVAL**
*sig* is invalid.
&nbsp;
- **EPERM**
The calling process has no permission to send the signal to the process group.
&nbsp;
- **ESRCH**
The target process group does not exist.

## Examples
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
