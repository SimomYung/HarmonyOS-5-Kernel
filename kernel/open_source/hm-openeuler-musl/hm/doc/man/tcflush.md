# tcflush()
Flush non-transmitted output data, non-read input data, or both

## Synopsis
```c
#include <termios.h>

int tcflush(int fd, int queue);
```

## Arguments

- **fd**
A file descriptor that refers to a terminal device.
&nbsp;
- **queue**
A symbolic constant that specifies which queue to flush. It is defined in &lt;termios.h&gt; and can be any of the following:
&nbsp;
  - **TCIFLUSH**
flushes the input queue, which contains data that have been received but not yet read.
&nbsp;
  - **TCOFLUSH**
flushes the output queue, which contains data that have been written but not yet transmitted.
&nbsp;
  - **TCIOFLUSH**
flushes both the input and output queue.


## Library
libc

## Description
The tcflush() function discards any data written to the terminal referenced by *fd* which has not been transmitted to the terminal, or any data received from the terminal but not yet read, depending on the value of *queue*.

## Returns

- **0**
Flush non-transmitted output data, non-read input data, or both successfully.
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
The *queue* argument is not a supported value.
&nbsp;
- **EIO**
The process group of the writing process is orphaned, the calling thread is not blocking SIGTTOU, and the process is not ignoring SIGTTOU.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
