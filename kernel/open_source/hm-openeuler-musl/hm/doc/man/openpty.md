# openpty()
Find an available pseudo-terminal

## Synopsis
```c
#include <pty.h>

int openpty(int *pm, int *ps, char *name, const struct termios *tio,
            const struct winsize *ws);
```

## Arguments

- **pm**
Integer pointer to a file descriptor on the master side.
&nbsp;
- **ps**
Integer pointer to a file descriptor on the slave side.
&nbsp;
- **name**
Char pointer to the filename of the slave, if it is non-null.
&nbsp;
- **tio**
A pointer to a termios structure, which will determine the terminal attributes of the slave side, if it is non-null.
&nbsp;
- **ws**
A pointer to a winsize structure, which will determine the window size of the slave side, if it is non-null.


## Library
libc

## Description
The openpty() function finds an available pseudo-terminal and returns file descriptors for the master and slave.

## Returns

- **0**
An available pseudo-terminal was found.
&nbsp;

- **-1**
An error occurred.

## Errors

- **ENOENT**
There are no available pseudo-terminals.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
