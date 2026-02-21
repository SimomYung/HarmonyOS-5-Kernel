# getpgid()
Get the process group ID of a process

## Synopsis
```c
#include <unistd.h>

pid_t getpgid(pid_t pid);
```

## Arguments

- **pid**
The process ID of the process to be queried.

## Library
libc

## Description

The getpgid() function returns the process group ID of the process specified by *pid*. If *pid* is zero, the process group ID of the calling process will be returned.

## Returns

- **pgid**
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
