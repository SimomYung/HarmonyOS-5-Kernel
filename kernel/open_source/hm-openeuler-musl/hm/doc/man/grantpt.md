# grantpt()
Grant access to the slave pseudo-terminal

## Synopsis
```c
#define _XOPEN_SOURCE
#include <stdlib.h>

int grantpt(int fd);
```

## Arguments

- **fd**
A file descriptor that refers to a master pseudo-terminal device.


## Library
libc

## Description
The current implementation always returns 0.

## Returns

Always returns 0.

## Errors

N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
