# ptsname\_r()
Get the name of a slave pseudo-terminal device

## Synopsis
```c
#include <stdlib.h>

int ptsname_r(int fd, char *buf, size_t len)
```

## Arguments

- **fd**
Master pseudo-terminal device file descriptor.
&nbsp;
- **buf**
A pointer to the buffer for the pathname of the slave pseudo-terminal.
&nbsp;
- **len**
The length of the buffer.


## Library
libc

## Description
The ptsname\_r() function gets the name of a slave pseudo-terminal device, eg: /dev/pts/0.

## Returns

- **0**
Successfully obtained slave pseudo-terminal name.
&nbsp;

- **-1**
An error occurred.

## Errors

- **EINVAL**
*buf* is NULL.
&nbsp;
- **ENOTTY**
*fd* does not refer to a pseudo-terminal master device.
&nbsp;
- **ERANGE**
*buf* is too small.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
