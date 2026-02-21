# fchdir()
Change current working directory

## Synopsis
```c
#include <unisted.h>

int fchdir(int fd);
```
## Arguments

- **fd**
An open file descriptor to the directory used to change current working directory to.

## Library
libc

## Description
The function fchdir() changes the current working directory of the calling process.


## Returns

- **0**
On success, this function return 0.
&nbsp;

- **-1**
On failure, this function return -1, and errno is set.

## Errors

- **EBADF**
The argument *fd* is not valid.
&nbsp;
- **ENOMEM**
Insufficient memory.
&nbsp;
- **EACCES**
Search permission is denied.

## Examples
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
