# getsid()
Get the process group ID of a session leader

## Synopsis
```c
#include <unistd.h>

pid_t getsid(pid_t pid);
```

## Arguments

- **pid**
The process ID of the process to be queried.

## Library
libc

## Description

The getsid() function returns the process group ID of the session leader of the process specified by *pid*. If *pid* is zero, the process group ID of the session leader of the calling process will be returned.

Permission check is not supported.

## Returns

- **sid**
The process group ID queried.
&nbsp;

- **-1**
An error occurred.

## Errors

- **ESRCH**
The *pid* argument is invalid or there is no process whose process ID is equal to *pid*.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
