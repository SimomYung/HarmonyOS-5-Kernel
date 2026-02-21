# \_exit()
Terminate the calling process

## Synopsis
```c
#include <unistd.h>

void _exit(int status);
```

## Arguments

- **status**
The value *status & 0xFF* will be returned as the process's exit code.

## Library
libc

## Description

The function _exit() terminates the calling process. All file descriptors opened in the calling process are closed. All of child processes of the calling process will be inherited by **init** process. The parent of the calling process will be delivered a SIGCHLD signal.

## Returns
N/A

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
