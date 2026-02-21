# ttyname\_r()
Get name of a terminal

## Synopsis
```c
#include <unistd.h>

int ttyname_r(int fd, char *buf, size_t len);
```

## Arguments

- **fd**
A file descriptor associated with the terminal device.
&nbsp;
- **buf**
Points to the character array for the pathname of the terminal.
&nbsp;
- **len**
The length of the character array.


## Library
libc

## Description
The ttyname\_r() function gets the name of a terminal device.

## Returns

- **0**
Successfully obtained the terminal name.
&nbsp;
- **errno**
An error occurred.

## Errors

- **EBADF**
The *fd* argument is not a valid file descriptor.
&nbsp;
- **ENOTTY**
The file associated with *fd* is not a terminal.
&nbsp;
- **EINVAL**
*buf* is NULL.
&nbsp;
- **ERANGE**
*buf* is too small.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
