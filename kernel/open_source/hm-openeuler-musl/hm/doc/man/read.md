# read()
Read data from a file descriptor

## Synopsis

```c
#include <unistd.h>

ssize_t read(int fd, void *buf, size_t count);
```

## Arguments

- **fd**
A file descriptor refers to a file or a device or a pipe and so on.
&nbsp;
- **buf**
The memory specified to stored the data read.
&nbsp;
- **count**
The size of *buf*.


## Library
libc

## Description
read() reads up to count bytes from file descriptor *fd* into the buffer *buf*. If the *fd* supports seeking, the read operation gets data from the current offset of the file, and the file offset is added by the count of bytes read.

## Returns

On success, return the number of bytes read.  On error, return -1 and errno is set.

## Errors

- **EFAULT**
The address of *buf* is not valid.
&nbsp;
- **EBADF**
The value of *fd* is not valid.
&nbsp;
- **EISDIR**
The *fd* refers to a directory.
&nbsp;
- **ESPIPE**
The file is a pipe, FIFO, or socket.
&nbsp;


## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
