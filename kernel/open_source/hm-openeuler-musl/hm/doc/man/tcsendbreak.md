# tcsendbreak()
Send a continuous stream of zero-valued bits for a specific duration

## Synopsis
```c
#include <termios.h>

int tcsendbreak(int fd, int duration);
```

## Arguments

- **fd**
A file descriptor that refers to a terminal device.
&nbsp;
- **duration**
The duration of zero-valued bits

## Library
libc

## Description
The tcsendbreak() function transmits a continuous stream of zero-valued bits for four-tenths of a second to the terminal referenced by *fd*. The duration parameter is ignored in this implementation.

## Returns

- **0**
Transmits zero-valued bits stream successfully.
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
