# getpgrp()
Get the process group ID of the calling process

## Synopsis
```c
#include <unistd.h>

pid_t getpgrp(void);
```

## Arguments
N/A

## Library
libc

## Description

The getpgrp() function returns the process group ID of the calling process.

## Returns

- **pgid**
The process group ID queried. The function getpgrp() always succeeds, so *pgid* will be returned.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
