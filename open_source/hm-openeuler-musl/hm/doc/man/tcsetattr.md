# tcsetattr()
Set the parameters associated with the terminal

## Synopsis
```c
#include <termios.h>

int tcsetattr(int fd, int actions, struct termios *tio);
```

## Arguments

- **fd**
A file descriptor that refers to a terminal device.
&nbsp;
- **actions**
How the changed status takes effect.
&nbsp;
- **tio**
A pointer to a termios structure.


## Library
libc

## Description
The tcsetattr() function set the parameters associated with the terminal referred to by the open file descriptor *fd* (an open file descriptor associated with a terminal) from the termios structure referenced by *tio* as follows:
- **TCSANOW**
The change occur immediately.
&nbsp;
- **TCSADRAIN**
The change occur after all output written to *fd* is transmitted. This function should be used when changing parameters that affect output.
&nbsp;
- **TCSAFLUSH**
The change occur after all output written to *fd* is transmitted, and all input so far received but not read be discarded before the change is made.

For supported attributes, please refer to termios.

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
&nbsp;
- **EINVAL**
The *actions* argument is not a supported value, or an attempt was made to change an attribute represented in the termios structure to an unsupported value.
&nbsp;
- **EINTR**
A signal interrupted *tcsetattr()*.
&nbsp;
- **EIO**
The process group of the writing process is orphaned, the calling thread is not blocking SIGTTOU, and the process is not ignoring SIGTTOU.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
