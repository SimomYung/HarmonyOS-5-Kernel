# ttyname()
Get name of a terminal

## Synopsis
```c
#include <unistd.h>

char *ttyname(int fd);
```

## Arguments

- **fd**
A file descriptor associated with the terminal device.


## Library
libc

## Description
The ttyname() function returns a pointer to a string containing a null-terminated pathname of the terminal associated with file descriptor *fd*.

## Returns

A pointer to a string is returned on success, and NULL on failure.

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
