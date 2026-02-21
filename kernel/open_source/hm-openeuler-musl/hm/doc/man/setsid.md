# setsid()
Create session and set process group ID

## Synopsis
```c
#include <unistd.h>

pid_t setsid(void)
```

## Arguments
N/A

## Library
libc

## Description

The setsid() function will create a session for the calling process. Upon success, the calling process will be the session leader and the only process of the session. Its session ID and process group ID will be set to its process ID. The controlling terminal will be unlinked if the calling process had one and setsid() succeeds.

## Returns

- **the new process group ID**
The setsid() function succeeds.
&nbsp;

- **-1**
An error occurred.

## Errors

- **EPERM**
The calling process is already a process group leader. Or there is another process whose process group ID matches the process ID of the calling process.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
