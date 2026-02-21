# getppid()
Get the parent process ID

## Synopsis
```c
#include <unistd.h>

pid_t getppid(void);
```

## Arguments
N/A

## Library
libc

## Description

The getppid() function returns the parent process ID of the calling process.

## Returns

- **ppid**
The parent process ID queried. The getppid() function is always successful.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
