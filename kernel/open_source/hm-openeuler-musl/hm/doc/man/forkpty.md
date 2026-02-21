# forkpty()
Create a new process operating in a pseudo-terminal

## Synopsis
```c
#include <pty.h>

int forkpty(int *pm, char *name, const struct termios *tio,
            const struct winsize *ws);
```

## Arguments

- **pm**
Points to the file descriptor of the master side of the pseudo-terminal.
&nbsp;
- **name**
Points to the filename of the slave, if it is non-null.
&nbsp;
- **tio**
Points to a termios structure, which will determine the terminal attributes of the slave side of the pseudo-terminal, if it is non-null.
&nbsp;
- **ws**
Points to a winsize structure, which will determine the window size of the slave side of the pseudo-terminal, if it is non-null.


## Library
libc

## Description
The forkpty() function creates a new process operating in a pseudo-terminal. The parent process will open the master pseudo-terminal, and the child process will open the slave-pseudo terminal.

## Returns

- **0**
The child process of the forkpty() returns 0.
&nbsp;

- **ID**
The parent process of the forkpty() returns the process ID of the child process.
&nbsp;

- **-1**
An error occurred.

## Errors

forkpty() fails if either openpty() or fork() fails.

## Example
&nbsp;

## Classification
UNIX

## Function Safety
TODO
