# isatty()
Test whether a file descriptor refers to a terminal

## Synopsis
```c
#include <unistd.h>

int isatty(int fd);
```

## Arguments

- **fd**
The file descriptor to test


## Library
libc

## Description
The isatty() function tests whether *fd* is an open file descriptor referring to a terminal.

## Returns

- **1**
*fd* is an open file descriptor referring to a terminal.
&nbsp;

- **0**
An error occurred.

## Errors

- **EBADF**
The *fd* argument is not a valid file descriptor.
&nbsp;
- **ENOTTY**
The file associated with *fd* is not a terminal.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
