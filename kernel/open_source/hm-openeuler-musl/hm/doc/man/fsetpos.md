# fsetpos()
Set the file-position

## Synopsis
```c
#include <stdio.h>

int fsetpos(FILE *stream, const fpos_t *pos);
```

## Arguments 
- **stream**
A pointer refers to the stream.
&nbsp;
- **pos**
The file position indicator for the stream is stored in *fpos_t* object pointed to by *pos*.

## Library
libc

## Description
The fsetpos() function sets the file-position and the state indicator for the stream pointed to by *stream* according to the value of the object pointed to by *pos*. When a read or write error occurs, the error indicator for the stream is set.

## Returns
- **0**
Set the file-position successfully.
&nbsp;
- **A None-zero value**
An error occurs, a none-zero is returned and *errno* is set.

## Errors
- **EAGAIN**
The O_NONBLOCK flag is set for the file descriptor and the thread would be delayed in file-position operation.
&nbsp;
- **EBADF**
The file descriptor underlying the stream file is not open for writing or the stream's buffer needed to be flushed and the file is not open.
&nbsp;
- **EFBIG**
Attempt to write to a file that exceeds the maximum file size, or attempt to write to a file that exceeds the file size of the process, or attempt to write to a file at or beyond the offset maximum.
&nbsp;
- **ENOSPC**
There is no free space on the device to store the file.
&nbsp;
- **ESPIPE**
The file descriptor underlying *stream* is associated with a pipe, FIFO or socket.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO

