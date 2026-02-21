# close()
Close a file descriptor

## Synopsis
```c
#include <unisted.h>

int close(int fd);
```
## Arguments

- **fd**
An open file descriptor.

## Library
libc

## Description
The function close() closes a file descriptor, so that the fd no longer refers to the file specified.


## Returns

- **0**
On success, this function return 0.
&nbsp;

- **-1**
On failure, this function return -1, and errno is set.

## Errors

- **EBADF**
*fd* is not a valid open file descriptor.
&nbsp;

## Examples
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
