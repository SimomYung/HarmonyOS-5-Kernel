# tcdrain()
Wait for transmission of output

## Synopsis
```c
#include <termios.h>

int tcdrain(int fd);
```

## Arguments

- **fd**
A file descriptor that refers to a terminal device.

## Library
libc

## Description
The tcdrain() function shall block until all output written to the object referred to by *fd* is transmitted.

## Returns

- **0**
Wait for transmission of output successful completion.
&nbsp;
- **-1**
An error occurred.

## Errors

- **EBADF**
The *fd* argument is not a valid file descriptor.
&nbsp;
- **ENOTTY**
The file associated with *fd* is not a terminal.
&nbsp;
- **EINTR**
A signal interrupted tcdrain().

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
