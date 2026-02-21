# setpgrp()
Set the process group ID

## Synopsis
```c
#include <unistd.h>

pid_t setpgrp(void);
```

## Arguments
N/A

## Library
libc

## Description

The setpgrp() function is the same as:
```c
setpgid(0, 0);
```
It will set the process group ID of the calling process to its process ID.


## Returns

- **0**
The setpgrp() function succeeds.
&nbsp;

- **-1**
An error occurred.

## Errors

- **EPERM**
The calling process is a session leader.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
