# tcgetattr()
Get the parameters associated with the terminal

## Synopsis
```c
#include <termios.h>

int tcgetattr(int fd, struct termios *tio);
```

## Arguments

- **fd**
A file descriptor that refers to a terminal device.
&nbsp;
- **tio**
A pointer to a termios structure.


## Library
libc

## Description
The tcgetattr() function get the parameters associated with the terminal referred to by *fd* and store them in the termios structure referenced by *tio*. For supported attributes, please refer to termios.

## Returns

- **0**
Successfully obtained the parameters associated with the terminal.
&nbsp;

- **-1**
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
