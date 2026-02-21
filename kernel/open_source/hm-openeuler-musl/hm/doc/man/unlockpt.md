# unlockpt()
Unlock a pseudo-terminal master/slave pair

## Synopsis
```c
#define _XOPEN_SOURCE
#include <stdlib.h>

int unlockpt(int fd);
```

## Arguments

- **fd**
A file descriptor associated with the master pseudo-terminal.


## Library
libc

## Description
The unlockpt() function unlocks the slave pseudo-terminal device corresponding to the master pseudo-terminal referred to by *fd*.

## Returns

- **0**
Successfully unlocked a pseudo-terminal master/slave pair.
&nbsp;

- **-1**
An error occurred.

## Errors

- **EBADF**
The *fd* argument is not a valid file descriptor.
&nbsp;
- **EINVAL**
The *fd* argument is not associated with a master pseudo-terminal.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
